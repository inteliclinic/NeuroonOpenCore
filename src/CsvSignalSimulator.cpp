#include "CsvSignalSimulator.h"
#include "Exceptions.h"
#include "NeuroonSignals.h"
#include <chrono>
#include <thread>
#include <sstream>
#include <stdio.h>

#define BIGGER(a,b) a>b?a:b
#define EMISSION_INTERVAL_MS(t) std::get<0>(t)
#define PIPE_UP(t) std::get<1>(t)

CsvEegFramesSource::CsvEegFramesSource(const std::string path, std::size_t frame_size) :
  _frame_size(frame_size) {

  // read csv
  // parse & check signal from csv
  _check_and_parse_csv(path);
}

void CsvEegFramesSource::_check_and_parse_csv(std::string path){

  auto csv_map = CsvReader::read_csv_with_headers_from_path(path);

  std::vector<InValue> samples;
  if (csv_map.find("signal") == csv_map.end()) {
    csv_map.clear();
    samples = CsvReader::read_csv_no_headers_from_path(path)[0];
    // throw std::invalid_argument("No signal column in the csv file.");

  }
  else{
    samples = csv_map["signal"];
  }

  for (std::size_t i = 0;i<samples.size()/_frame_size;i++) {
    EegFrame frame;
    for(std::size_t j=0;j<_frame_size;j++){
      auto & iv = samples[i*_frame_size + j];
      if(iv.type() != InValue::Type::LLONG){
        std::stringstream ss;
        ss << "Invalid signal value: " << iv << " at index: " << i;
        throw std::invalid_argument(ss.str());
      }
      frame.signal[j] = iv.llong_value();
    }
    _frames.push_back(frame);
  }
}


////////////////////////////////////////// CsvSignalSimulator


CsvSignalSimulator::CsvSignalSimulator(){
  _set_timestamp_to_now();
}

void CsvSignalSimulator::_set_timestamp_to_now(){
  auto dtn = std::chrono::high_resolution_clock::now().time_since_epoch();
  // assuming nanoseconds.....
  _starting_timestamp = dtn.count() / 1000000;
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
  // while requested time hasnt passed

  // max time to wait
  llong max_time_to_wait = 0;
  for(const auto & triple : _pipes){
    max_time_to_wait = BIGGER(EMISSION_INTERVAL_MS(triple), (ullong)max_time_to_wait);
  }

  while(ms_left > 0 || ms_to_simulate==0){
    bool should_continue = false;
    for(const auto & t : _pipes){
      if(!PIPE_UP(t)->is_exhausted()) should_continue = true;
    }
    if(!should_continue) break;

    auto start = std::chrono::high_resolution_clock::now();
    // find sleep period for next frame emission

    int time_to_wait = max_time_to_wait;
    std::vector<std::tuple<uint, std::unique_ptr<IFrameStreamPipe>>*> minpipes;
    for(auto & t : _pipes){
      if(PIPE_UP(t)->is_exhausted()){
        // this pipe is exhausted
        continue;
      }

      llong emission_time = EMISSION_INTERVAL_MS(t) - (_time_passed % EMISSION_INTERVAL_MS(t));
      if(emission_time==0) emission_time = EMISSION_INTERVAL_MS(t);
      // printf("emmission time %lld%%%d %d\n",_time_passed, ss->ms_per_frame(),emission_time);
      if(emission_time < time_to_wait){
        time_to_wait = emission_time;
        minpipes.clear();
        minpipes.push_back(&t);
      }
      // frames for all sources should be emitted when necessary
      else if(emission_time == time_to_wait){
        minpipes.push_back(&t);
      }
    }

    // theres not enough time for emission
    if(ms_to_simulate !=0 && (ullong)time_to_wait > ms_left){
      // wait only the time we can
      time_to_wait = ms_left;
      // clear selected sources - don't emit frames
      minpipes.clear();
    }

    // Wait correct amount of time (tries to include the delay caused by computation)
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end-start;
    llong ttw = BIGGER(0,round(time_passing_modifier*time_to_wait - elapsed.count()));
    std::this_thread::sleep_for(std::chrono::milliseconds((llong)ttw));
    _time_passed += time_to_wait;
    // printf("Sleeping for: %lldms. Passing: %dms.\n", ttw, time_to_wait);

    // Now the time has passed, so we mark it.
    ms_left -= time_to_wait;

    // transmit frames in selected pipes
    for(auto & t : minpipes){
      ullong ts = _starting_timestamp + _time_passed - EMISSION_INTERVAL_MS(*t);
      // Add frame to prepared for emission frames
      LOG(INFO) << "Emiting frame from pipe: " << PIPE_UP(*t).get();
      PIPE_UP(*t)->pass_next_frame_with_timestamp(ts);
    }
  }
}

void CsvSignalSimulator::add_streaming_pipe(std::unique_ptr<IFrameStreamPipe> & pipe,
                                            uint pipe_frame_emission_interval_ms){
  _pipes.push_back(std::make_tuple(pipe_frame_emission_interval_ms, std::move(pipe)));
}
