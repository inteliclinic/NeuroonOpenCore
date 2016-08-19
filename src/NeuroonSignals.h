#ifndef __NEUROON_SIGNALS__
#define __NEUROON_SIGNALS__

#include "CommonTypedefs.h"
#include <vector>
#include <map>
#include "VectorView.h"
#include "InValue.h"


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
 public:

  SignalSpec(SignalOrigin signal_origin, int sampling_rate) :
    _sampling_rate(sampling_rate), _signal_origin(signal_origin) {}

  SignalOrigin signal_origin() const { return _signal_origin;}

  // sampling rate of signal in Hz
  int sampling_rate() const { return _sampling_rate; }

  // time interval between two samples of signal in miliseconds
  int ms_per_sample() const { return (int)(1000.0 / this->sampling_rate()); }

};


class NeuroonSignals{

  static const std::map<SignalOrigin, SignalSpec> _signal_specs;

  std::map<SignalOrigin, std::vector<double>> _signals = {
    {SignalOrigin::EEG,           {}},
    {SignalOrigin::IR_LED,        {}},
    {SignalOrigin::ACCELEROMETER, {}},
    {SignalOrigin::TEMPERATURE,   {}}
  };

public:

  // receive frame of data
  void receive_frame(SignalFrame * frame);

  // get vectors of received samples
  const std::vector<double> & operator[](SignalOrigin ss) const {
    return _signals.at(ss);
  }

  // get specification of signals by its source
  static const SignalSpec & specs(SignalOrigin ss){
    return _signal_specs.at(ss);
  }

  // This is intended to provide information about number of already received
  // samples. The implementation may change as we might want not to store all the samples
  // in the vectors.
  std::size_t received_samples(SignalOrigin ss) const{
    return _signals.at(ss).size();
  }

};

#endif
