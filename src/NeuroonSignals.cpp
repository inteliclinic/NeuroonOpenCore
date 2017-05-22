#include "NeuroonSignals.h"
#include "Constants.h"
#include "logger.h"
#include <cmath>
#include <algorithm>
// --------------

#define TOTAL_COUNT(t) std::get<0>(t)
#define LAST_TS(t) std::get<1>(t)
#define SIGNAL_VEC(t) std::get<2>(t)

// -------------- PUBLIC API -----------------------

const std::vector<double> & NeuroonSignals::eeg_signal() const { return SIGNAL_VEC(_eeg_signal); }
const std::vector<double> & NeuroonSignals::ir_led_signal() const { return SIGNAL_VEC(_ir_led_signal); }
const std::vector<double> & NeuroonSignals::red_led_signal() const { return SIGNAL_VEC(_red_led_signal); }
const std::vector<Double3d> & NeuroonSignals::accel_axes_signal() const { return SIGNAL_VEC(_accel_axes_signal); }
const std::vector<double> & NeuroonSignals::temperature_signal() const { return SIGNAL_VEC(_temperature_signal); }


void NeuroonSignals::clear_data(){
  _eeg_signal = std::make_tuple(0,0,std::vector<double>());
  _ir_led_signal = std::make_tuple(0,0,std::vector<double>());
  _red_led_signal = std::make_tuple(0,0,std::vector<double>());
  _accel_axes_signal = std::make_tuple(0,0,std::vector<Double3d>());
  _temperature_signal = std::make_tuple(0,0,std::vector<double>());
}

ullong NeuroonSignals::last_timestamp(SignalOrigin so) const {
  switch(so){
  case SignalOrigin::EEG:
    return LAST_TS(_eeg_signal);
  case SignalOrigin::IR_LED:
    return LAST_TS(_ir_led_signal);
  case SignalOrigin::RED_LED:
    return LAST_TS(_red_led_signal);
  case SignalOrigin::ACCELEROMETER:
    return LAST_TS(_accel_axes_signal);
  case SignalOrigin::TEMPERATURE:
    return LAST_TS(_temperature_signal);
  }
}

std::size_t NeuroonSignals::total_signal_samples(SignalOrigin so) const {
  switch(so){
  case SignalOrigin::EEG:
    return TOTAL_COUNT(_eeg_signal);
  case SignalOrigin::IR_LED:
    return TOTAL_COUNT(_ir_led_signal);
  case SignalOrigin::RED_LED:
    return TOTAL_COUNT(_red_led_signal);
  case SignalOrigin::ACCELEROMETER:
    return TOTAL_COUNT(_accel_axes_signal);
  case SignalOrigin::TEMPERATURE:
    return TOTAL_COUNT(_temperature_signal);
  }
}


// receive frame of data

void NeuroonSignals::consume(std::shared_ptr<EegFrame> frame){

  LOG(DEBUG) << "Received eeg signal frame with timestamp: " << frame->timestamp;

  auto & signal = SIGNAL_VEC(_eeg_signal);
  auto ms_per_sample = _signal_specs.at(SignalOrigin::EEG).ms_per_sample();

  //if lost
  // TODO
  auto old_sz = signal.size();
  if (false){
    std::size_t lost_frames_count = 0;
    EegHoleFillingArgs args = {signal, lost_frames_count, frame };
    if(_eeg_lost_frame_hole_filling_function != nullptr){
      _eeg_lost_frame_hole_filling_function(args);
    }
    else{
      _default_nan_filling_eeg(args);
    }
  }

  // insert new data
  signal.insert(signal.end(),frame->signal, frame->signal + frame->Length);
  TOTAL_COUNT(_eeg_signal) += signal.size() - old_sz;
  LAST_TS(_eeg_signal) = frame->timestamp + std::max(static_cast<std::size_t>(0), frame->Length - 1) * ms_per_sample;
}

void NeuroonSignals::consume(std::shared_ptr<PatFrame> frame){
  LOG(DEBUG) << "Received PAT signal frame with timestamp: " << frame->timestamp;

  // ir led
  auto & ir_signal = SIGNAL_VEC(_ir_led_signal);
  auto & redled_signal = SIGNAL_VEC(_red_led_signal);
  auto & accel_axes_signal = SIGNAL_VEC(_accel_axes_signal);
  auto & temperature_signal = SIGNAL_VEC(_temperature_signal);

  // given present situation all signals from this frame has same signal sampling frequency
  // auto ms_per_sample = _signal_specs.at(SignalOrigin::IR_LED).ms_per_sample();

  //if lost
  // TODO
  auto ir_old_sz = ir_signal.size();
  auto redled_old_sz = redled_signal.size();
  auto accelaxes_old_sz = accel_axes_signal.size();
  auto temperature_old_sz = temperature_signal.size();
  if (false){
    // std::size_t lost_frames_count = 0;
    // PatHoleFillingArgs args = {signal, lost_frames_count, &frame };
    // if(_eeg_lost_frame_hole_filling_function != nullptr){
    //   _eeg_lost_frame_hole_filling_function(args);
    // }
    // else{
    //   _default_nan_filling_eeg(args);
    // }
  }

  // insert new data
  ir_signal.push_back((double)frame->ir_led);
  redled_signal.push_back((double)frame->red_led);

  accel_axes_signal.push_back({
      (double)frame->accel_axes.x,
      (double)frame->accel_axes.y,
      (double)frame->accel_axes.z});
  temperature_signal.push_back((double) std::max(frame->temperature[0],frame->temperature[1]));


  TOTAL_COUNT(_ir_led_signal) += ir_signal.size() - ir_old_sz;
  LAST_TS(_ir_led_signal) = frame->timestamp;

  TOTAL_COUNT(_red_led_signal) += redled_signal.size() - redled_old_sz;
  LAST_TS(_red_led_signal) = frame->timestamp;

  TOTAL_COUNT(_accel_axes_signal) += accel_axes_signal.size() - accelaxes_old_sz;
  LAST_TS(_accel_axes_signal) = frame->timestamp;

  TOTAL_COUNT(_temperature_signal) += temperature_signal.size() - temperature_old_sz;
  LAST_TS(_temperature_signal) = frame->timestamp;
}

// -------------- STATIC DATA ----------------------

const std::map<SignalOrigin, SignalSpec> NeuroonSignals::_signal_specs = {
  {SignalOrigin::EEG,           SignalSpec(SignalOrigin::EEG, 125)},
  {SignalOrigin::RED_LED,        SignalSpec(SignalOrigin::IR_LED, 25)},
  {SignalOrigin::IR_LED,        SignalSpec(SignalOrigin::IR_LED, 25)},
  {SignalOrigin::ACCELEROMETER, SignalSpec(SignalOrigin::ACCELEROMETER, 25)},
  {SignalOrigin::TEMPERATURE,   SignalSpec(SignalOrigin::TEMPERATURE, 1)}
};

// -------------- PRIVATE --------------------------


void NeuroonSignals::_default_nan_filling_eeg(EegHoleFillingArgs args){

  LOG(INFO) << "Filling " << args.lost_frames_count << " lost frames with nans.";
  auto new_size = args.gathered_eeg_signal.size() + args.lost_frames_count * EegFrame::Length;
  args.gathered_eeg_signal.resize(new_size, std::nan(NO_DATA_TAG));
}

void NeuroonSignals::_default_nan_filling_accelledstemp(PatHoleFillingArgs args){
  auto l = args.lost_frames_count;
  auto n = std::nan(NO_DATA_TAG);
  LOG(INFO) << "Filling " << l << " lost frames with nans.";
  args.gathered_ir_led_signal.resize(args.gathered_ir_led_signal.size() + l, n);
  args.gathered_red_led_signal.resize(args.gathered_red_led_signal.size() + l, n);
  args.gathered_temperature_signal.resize(args.gathered_temperature_signal.size() + l, n);
  args.gathered_accel_axes_signal.resize(args.gathered_ir_led_signal.size() + l, {n,n,n});
}
