#ifdef DESKTOP_BUILD

#include "SignalSimulator.h"
#include "Exceptions.h"
#include "NeuroonSignals.h"
#include <chrono>
#include <thread>
#include <sstream>
#include <stdio.h>

#define EMISSION_INTERVAL_MS(t) std::get<0>(t)
#define PIPE_UP(t) std::get<1>(t)


EegFramesSource::EegFramesSource(SignalSource<std::int16_t> signal_source, std::size_t frame_size) :
  _frame_size(frame_size) {

  auto samples = signal_source.get_values();

  for (std::size_t i = 0;i<samples.size()/_frame_size;i++) {
    EegFrame frame;
    for(std::size_t j=0;j<_frame_size;j++){
      frame.signal[j] = samples[i*_frame_size + j];
    }
    _frames.push_back(frame);
  }
}

////////////////////////////////////////// PatFrame


PatFramesSource::PatFramesSource (SignalSource<std::int32_t> ir_led,
                                                    SignalSource<std::int32_t> red_led,
                                                    SignalSource<std::int16_t> accel_x,
                                                    SignalSource<std::int16_t> accel_y,
                                                    SignalSource<std::int16_t> accel_z,
                                                    SignalSource<std::int8_t> temperature_1,
                                                    SignalSource<std::int8_t> temperature_2){

  // gather all csv based sources
  std::vector<SourceSpec> specs = {
    ir_led.spec(),
    red_led.spec(),
    accel_x.spec(),
    accel_y.spec(),
    accel_z.spec(),
    temperature_1.spec(),
    temperature_2.spec(),
  };

  VectorView<std::int32_t> led_signals[2] = {{},{}};
  VectorView<std::int16_t> accel_axes[3] ={{},{},{}};
  VectorView<std::int8_t> temperatures[2] = {{},{}};

  std::size_t final_source_length = std::numeric_limits<std::size_t>::max();
  bool any_csv_source = false;

  // csv sources ---------------------------

  // adjust source length to the shorted csv file.

  // ---------------------

  if(ir_led.spec().option() == SourceSpec::SourceOption::CSV){
    auto & v = led_signals[0];
    v = ir_led.get_values();
    any_csv_source = true;
    if(v.size() < final_source_length) { final_source_length = v.size();}
  }
  if(red_led.spec().option() == SourceSpec::SourceOption::CSV){
    auto & v = led_signals[1];
    any_csv_source = true;
    v = red_led.get_values();
    if(v.size() < final_source_length) { final_source_length = v.size();}
  }
  if(accel_x.spec().option() == SourceSpec::SourceOption::CSV){
    any_csv_source = true;
    auto & v = accel_axes[0];
    v = accel_x.get_values();
    if(v.size() < final_source_length) { final_source_length = v.size();}
  }
  if(accel_y.spec().option() == SourceSpec::SourceOption::CSV){
    any_csv_source = true;
    auto & v = accel_axes[1];
    v = accel_y.get_values();
    if(v.size() < final_source_length) { final_source_length = v.size();}

  }
  if(accel_z.spec().option() == SourceSpec::SourceOption::CSV){
    any_csv_source = true;
    auto & v = accel_axes[2];
    v = accel_z.get_values();
    if(v.size() < final_source_length) { final_source_length = v.size();}
  }
  if(temperature_1.spec().option() == SourceSpec::SourceOption::CSV){
    any_csv_source = true;
    auto & v = temperatures[0];
    v = temperature_1.get_values();
    if(v.size() < final_source_length) { final_source_length = v.size();}
  }
  if(temperature_2.spec().option() == SourceSpec::SourceOption::CSV){
    any_csv_source = true;
    auto & v = temperatures[1];
    v = temperature_2.get_values();
    if(v.size() < final_source_length) { final_source_length = v.size();}
  }


  // non csv sources ---------------------------

  // if there was no csv sources, then adjust source length to the longest default size
  // of single signal sources

  // ---------------------

  if(!any_csv_source){
    final_source_length = std::numeric_limits<std::size_t>::lowest();
    for(auto & spc : specs){
      if(spc.default_size() > final_source_length) { final_source_length = spc.default_size();}
    }
  }

  if(ir_led.spec().option() != SourceSpec::SourceOption::CSV){
    auto & v = led_signals[0];
    v = ir_led.get_values(final_source_length);
  }

  if(red_led.spec().option() != SourceSpec::SourceOption::CSV){
    auto & v = led_signals[1];
    v = red_led.get_values(final_source_length);
  }

  if(accel_x.spec().option() != SourceSpec::SourceOption::CSV){
    auto & v = accel_axes[0];
    v = accel_x.get_values(final_source_length);
  }

  if(accel_y.spec().option() != SourceSpec::SourceOption::CSV){
    auto & v = accel_axes[1];
    v = accel_y.get_values(final_source_length);

  }
  if(accel_z.spec().option() != SourceSpec::SourceOption::CSV){
    auto & v = accel_axes[2];
    v = accel_z.get_values(final_source_length);
  }
  if(temperature_1.spec().option() != SourceSpec::SourceOption::CSV){
    auto & v = temperatures[0];
    v = temperature_1.get_values(final_source_length);
  }
  if(temperature_2.spec().option() != SourceSpec::SourceOption::CSV){
    auto & v = temperatures[1];
    v = temperature_2.get_values(final_source_length);
  }

  for(int i = 0 ; i < final_source_length ; i++) {
    PatFrame al;
    al.timestamp = 0;
    al.ir_led = led_signals[0][i];
    al.red_led = led_signals[1][i];

    AccelAxes ax;

    ax.x = accel_axes[0][i];
    ax.y = accel_axes[1][i];
    ax.z = accel_axes[2][i];

    al.accel_axes = ax;

    al.temperature[0] = temperatures[0][i];
    al.temperature[1] = temperatures[1][i];

    _frames.push_back(al);
  }
}

////////////////////////////////////////// SignalSimulator


SignalSimulator::SignalSimulator(){
  _set_timestamp_to_now();
}

void SignalSimulator::_set_timestamp_to_now(){
  auto dtn = std::chrono::high_resolution_clock::now().time_since_epoch();
  // assuming nanoseconds.....
  _starting_timestamp = dtn.count() / 1000000;
}


bool SignalSimulator::pass_time(ullong ms_to_simulate,
                                double time_passing_modifier,
                                bool force_wait){

  ullong ms_left = ms_to_simulate;
  // while requested time hasnt passed

  // max time to wait
  llong max_time_to_wait = 0;
  for(const auto & triple : _pipes){
    max_time_to_wait = std::max<ullong>(EMISSION_INTERVAL_MS(triple), (ullong)max_time_to_wait);
  }

  bool all_exhausted = false;
  while(ms_left > 0 || ms_to_simulate==0){
    all_exhausted = false;
    for(const auto & t : _pipes){
      if(!PIPE_UP(t)->is_exhausted()) all_exhausted = true;
    }
    if(!all_exhausted && !force_wait){
      break;
    }

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
    llong ttw = std::max<llong>(0,round(time_passing_modifier*time_to_wait - elapsed.count()));
    if(ttw != 0){
      std::this_thread::sleep_for(std::chrono::milliseconds((llong)ttw));
    }
    _time_passed += time_to_wait;
    // printf("Sleeping for: %lldms. Passing: %dms.\n", ttw, time_to_wait);

    // Now the time has passed, so we mark it.
    ms_left -= time_to_wait;

    // transmit frames in selected pipes
    for(auto & t : minpipes){
      ullong ts = _starting_timestamp + _time_passed - EMISSION_INTERVAL_MS(*t);
      // Add frame to prepared for emission frames
      LOG(DEBUG) << "Emiting frame from pipe: " << PIPE_UP(*t).get();
      PIPE_UP(*t)->pass_next_frame_with_timestamp(ts);
    }
  }
  return all_exhausted;
}

void SignalSimulator::add_streaming_pipe(std::unique_ptr<IFrameStreamPipe> pipe,
                                            uint pipe_frame_emission_interval_ms){
  _pipes.push_back(std::make_tuple(pipe_frame_emission_interval_ms, std::move(pipe)));
}

#endif
