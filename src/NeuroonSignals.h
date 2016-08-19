#ifndef __NEUROON_SIGNALS__
#define __NEUROON_SIGNALS__

#include "CommonTypedefs.h"
#include <vector>
#include <map>
#include <functional>
#include "VectorView.h"
#include "Signal.h"
#include "DataSink.h"

class NeuroonSignals : public SignalFrameDataSink{

  static const std::map<SignalOrigin, SignalSpec> _signal_specs;
  std::function<void (std::vector<double>&, const VectorView<int>&, ullong,ullong,ullong)> _filling_hole_function = nullptr;

  std::map<SignalOrigin, std::vector<double>> _signals = {
    {SignalOrigin::EEG,           {}},
    {SignalOrigin::IR_LED,        {}},
    {SignalOrigin::ACCELEROMETER, {}},
    {SignalOrigin::TEMPERATURE,   {}}
  };

  std::map<SignalOrigin, ullong> _last_timestamps = {
    {SignalOrigin::EEG,           0},
    {SignalOrigin::IR_LED,        0},
    {SignalOrigin::ACCELEROMETER, 0},
    {SignalOrigin::TEMPERATURE,   0}
  };

  // default filling function
  void fill_with_nans(std::vector<double> & signal,
                      const VectorView<int> &new_data,
                      ullong begin_hole,
                      ullong end_hole,
                      ullong ms_per_sample);
public:

  NeuroonSignals() : SignalFrameDataSink() {}

  // sets hole filling function,
  // the function should accept index of last samples before the hole,
  // vector with signal, till the beginning of the hole
  // vectorview  of signal just aquired (marking the end of the hole)
  // timestamps of the last sample befor the hole
  // timestamps of newly aquired samples (end of the hole),
  // ms_per_sample parameter of the signal
  // basing on that it should fill the hole in the signal passed as first
  // parameter
  // it defaults to filling it with std::nan
  void set_hole_filling_function(std::function<void (std::vector<double>&, const VectorView<int>&, ullong,ullong,ullong)> f){
    _filling_hole_function = f;
  }

  void consume(SignalFrame * frame) override{
    receive_frame(frame);
  }
  // receive frame of data
  void receive_frame(SignalFrame * frame);

  // get vectors of received samples
  const std::vector<double> & operator[](SignalOrigin ss) const {
    return _signals.at(ss);
  }

  ullong last_timestamp(SignalOrigin so) const {
    return _last_timestamps.at(so);
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
