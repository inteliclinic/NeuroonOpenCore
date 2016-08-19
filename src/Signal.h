#ifndef __SIGNAL__
#define __SIGNAL__

#include "CommonTypedefs.h"
#include "VectorView.h"


// source of signal in Neuroon Mask
enum SignalOrigin{ EEG, ACCELEROMETER, IR_LED, TEMPERATURE};

// structure holding data from the mask
struct SignalFrame{
  SignalOrigin origin;
  ullong timestamp;
  VectorView<int> signal;
};

// class holding characteristics of signal
class SignalSpec{
 private:
  int _sampling_rate;
  SignalOrigin _signal_origin;

  int compute_lost_frames_count(SignalFrame* frame);
 public:

 SignalSpec(SignalOrigin signal_origin, int sampling_rate) :
  _sampling_rate(sampling_rate), _signal_origin(signal_origin) {}

  SignalOrigin signal_origin() const { return _signal_origin;}

  // sampling rate of signal in Hz
  int sampling_rate() const { return _sampling_rate; }

  // time interval between two samples of signal in miliseconds
  int ms_per_sample() const { return (int)(1000.0 / this->sampling_rate()); }
};

#endif
