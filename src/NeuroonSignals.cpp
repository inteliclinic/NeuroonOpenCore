#include "NeuroonSignals.h"
#include "Constants.h"
#include <cmath>

const std::map<SignalOrigin, SignalSpec> NeuroonSignals::_signal_specs = {
  {SignalOrigin::EEG,           SignalSpec(SignalOrigin::EEG, 125)},
  {SignalOrigin::IR_LED,        SignalSpec(SignalOrigin::IR_LED, 25)},
  {SignalOrigin::ACCELEROMETER, SignalSpec(SignalOrigin::ACCELEROMETER, 25)},
  {SignalOrigin::TEMPERATURE,   SignalSpec(SignalOrigin::TEMPERATURE, 1)}
};

void NeuroonSignals::fill_with_nans(std::vector<double> & signal,
                                    const VectorView<int> &,
                                    ullong begin_hole,
                                    ullong end_hole,
                                    ullong ms_per_sample){

  auto samples_count = (end_hole - begin_hole)/ms_per_sample - 1;
  signal.resize(signal.size()+samples_count,std::nan(NO_DATA_TAG));
}

void NeuroonSignals::receive_frame(SignalFrame * frame){

  auto origin = frame->origin;

  // add nan check if this frame is next or something was lost between last and this
  auto last_sample_timestamp = _last_timestamps.at(origin);
  auto end_hole_timestamp = frame->timestamp;
  auto & new_data = frame->signal;
  auto & signal = _signals.at(origin);

  auto ms_per_sample = NeuroonSignals::specs(origin).ms_per_sample();

  if(_filling_hole_function == nullptr){
    fill_with_nans(signal, new_data, last_sample_timestamp, end_hole_timestamp, ms_per_sample);
  }
  else{
    _filling_hole_function(signal, new_data, last_sample_timestamp, end_hole_timestamp, ms_per_sample);
  }

  // update last timestamp as it changed
  _last_timestamps[origin] = ((new_data.size()-1) * ms_per_sample) + end_hole_timestamp;

  // insert new_data at the end of signal
  signal.insert(signal.end(),new_data.begin(),new_data.end());

}
