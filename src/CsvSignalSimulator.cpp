#include "CsvSignalSimulator.h"
#include "Exceptions.h"
#include "NeuroonSignals.h"
#include <chrono>
#include <thread>
#include <sstream>
#include <stdio.h>

#define BIGGER(a,b) a>b?a:b


CsvSignalSource::CsvSignalSource(std::string path, SignalSpec spec, int ms_per_frame) :
  _spec(spec),
  _ms_per_frame(ms_per_frame) {

  // read csv
  // parse & check signal from csv
  check_and_parse_csv(path);
}

void CsvSignalSource::check_and_parse_csv(std::string path){

  auto csv_map = CsvReader::read_csv_with_headers_from_path(path);

  std::vector<InValue> samples;
  if(csv_map.find("signal") == csv_map.end()){
    csv_map.clear();
    samples = CsvReader::read_csv_no_headers_from_path(path)[0];
    // throw std::invalid_argument("No signal column in the csv file.");

  }
  else{
    samples = csv_map["signal"];
  }

  std::size_t i = 0;
  for(auto & iv : samples){
    if(iv.type() != InValue::Type::LLONG){
      std::stringstream ss;
      ss << "Invalid signal value: " << iv << " at index: " << i;
      throw std::invalid_argument(ss.str());
    }
    else{
      this->_signal.push_back(iv.llong_value());
    }
    i++;
  }
}

CsvSignalSource CsvSignalSource::eeg_signal_source(std::string path, int ms_per_frame){
  return CsvSignalSource(path,NeuroonSignals::specs(SignalOrigin::EEG), ms_per_frame);
}

CsvSignalSource CsvSignalSource::irled_signal_source(std::string path, int ms_per_frame){
  return CsvSignalSource(path,NeuroonSignals::specs(SignalOrigin::IR_LED), ms_per_frame);
}

////////////////////////////////////////// CsvSignalSimulator
void CsvSignalSimulator::reset(){
  _time_passed = 0;
  _signal_bookmarks.clear();
}


/** this function simulate passage of time and emits frame of data according
*   to provided data frame sinks.
*   \param ms_to_simulate this many ms will be simulated.
*                         Pass 0 for emiting entire data.
*   \param time_passing_modifier rate at which real time passes.
*                                 0 - passing take not real time
*                                 1 - passing take normal time
*
*   \warning This function may actually emit the data a bit slower than expected
*/
void CsvSignalSimulator::pass_time(ullong ms_to_simulate,
                                   double time_passing_modifier){


  ullong ms_left = ms_to_simulate;
  // while requested time hasnt assed

  // max time to wait
  llong max_time_to_wait = 0;
  for(const auto & ss : this->sources){
    max_time_to_wait = BIGGER(ss->ms_per_frame(), max_time_to_wait);
  }

  while(ms_left > 0 || ms_to_simulate==0){
    bool should_continue = false;
    for(const auto & ss : this->sources){
      if(_signal_bookmarks[ss].second < ss->signal().size()) should_continue = true;
    }
    if(!should_continue) break;

    auto start = std::chrono::high_resolution_clock::now();
    // find sleep period for next frame emission

    int time_to_wait = max_time_to_wait;
    std::vector<CsvSignalSource*> minss;
    for(auto & ss : this->sources){
      if(_signal_bookmarks[ss].second >= ss->signal().size()){
        // source depleted
        continue;
      }


      int emission_time = ss->ms_per_frame() - (_time_passed % ss->ms_per_frame());
      if(emission_time==0) emission_time = ss->ms_per_frame();
      // printf("emmission time %lld%%%d %d\n",_time_passed, ss->ms_per_frame(),emission_time);
      if(emission_time < time_to_wait){
        time_to_wait = emission_time;
        minss.clear();
        minss.push_back(ss);
      }
      // frames for all sources should be emitted when necessary
      else if(emission_time == time_to_wait){
        minss.push_back(ss);
      }
    }

    // theres not enough time for emission
    if(ms_to_simulate !=0 && (ullong)time_to_wait > ms_left){
      // wait only the time we can
      time_to_wait = ms_left;
      // clear selected sources - don't emit frames
      minss.clear();
    }

    // prepare frames
    std::vector<SignalFrame> frames = {};

    // Time passes, we should remember how much time since
    // last emissed samples passed
    for(auto &ss : this->sources){
      _signal_bookmarks[ss].first += time_to_wait;
    }

    // Emit frame from selected sources
    for(auto & ss : minss){
      auto & sb = _signal_bookmarks[ss];
      auto ms_per_sample = ss->spec().ms_per_sample();

      // sb.first is time since last emissed sample
      auto samples_count = sb.first / ms_per_sample;

      // Notice we don't reset it to zero, as sometimes
      // ms_per_sample doesnt divide ms_per frame.
      // so we leave rest for accummulation.
      sb.first -= samples_count * ms_per_sample;

      // Source almost depleted
      if(sb.second + samples_count >= ss->signal().size()){
        samples_count = BIGGER(0,ss->signal().size() - sb.second);
        if(samples_count <= 0){
          // Source depleted, switch to next
          continue;
        }
      }

      // Starting index of emitted frame data.
      std::vector<int>::const_iterator b = ss->signal().begin()+ sb.second;

      // Timestamp of a frame is now (assumption)
      auto end = std::chrono::high_resolution_clock::now();
      auto dtn = end.time_since_epoch();
      ullong ts = dtn.count();

      // printf("Time passed: %lld", _time_passed);
      // printf("----------\n");
      // for(auto it=b;it!=b+samples_count;it++){
      //   printf("%d, ",*it);
      // }
      // printf("\n----------\n");

      // Add frame to prepared for emission frames
      frames.push_back({ ss->spec().signal_origin(), ts, {b, b+samples_count}});
      sb.second += samples_count;
    }

    // Wait correct amount of time (tries to include the delay caused by computation)
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end-start;
    llong ttw = BIGGER(0,round(time_passing_modifier*time_to_wait - elapsed.count()));
    std::this_thread::sleep_for(std::chrono::milliseconds((llong)ttw));
    this->_time_passed += time_to_wait;
    // printf("Sleeping for: %lldms. Passing: %dms.\n", ttw, time_to_wait);

    // Now the time has passed, so we mark it.
    ms_left -= time_to_wait;

    // and emit prepared frame to sinks
    for(auto & f : frames){
      // printf("\npushing new frame %s at time: %llu\n",f.origin == SignalOrigin::EEG ? "EEG" : "IRLED", _time_passed);
      for(auto & sink : this->sinks){
        sink->consume(&f);
      }
    }

  }
}
