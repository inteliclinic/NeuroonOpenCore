#ifndef __NEUROON_SIGNALS__
#define __NEUROON_SIGNALS__

#include "CommonTypes.h"
#include <vector>
#include <map>
#include <memory>
#include <functional>
#include <tuple>
#include "VectorView.h"
#include "DataSink.h"
#include "NeuroonSignalFrames.h"
#include "SignalTypes.h"


class INeuroonSignals {
public:
  virtual ~INeuroonSignals(){}

  virtual const std::vector<double> & eeg_signal() const = 0;
  virtual const std::vector<double> & ir_led_signal() const = 0;
  virtual const std::vector<double> & red_led_signal() const = 0;
  virtual const std::vector<Double3d> & accel_axes_signal() const = 0;
  virtual const std::vector<double> & temperature_signal() const = 0;

  // ask for last sample timestamp for each signal
  virtual ullong last_timestamp(SignalOrigin so) const = 0;

  // This is intended to provide information about number of already received
  // samples. The implementation may change as we might want not to store all the samples
  // in the vectors.
  virtual std::size_t total_signal_samples(SignalOrigin ss) const = 0;
};

class NeuroonSignals : public INeuroonSignals, public IDataSinkSp<EegFrame>, public IDataSinkSp<PatFrame>{
public:


  // ----------- lost frame signal hole filling

  struct EegHoleFillingArgs{
    std::vector<double> & gathered_eeg_signal;
    std::size_t lost_frames_count;
    const std::shared_ptr<const EegFrame> new_data;
  };

  struct PatHoleFillingArgs{
    std::vector<Double3d> & gathered_accel_axes_signal;
    std::vector<double> & gathered_ir_led_signal;
    std::vector<double> & gathered_red_led_signal;
    std::vector<double> & gathered_temperature_signal;
    std::size_t lost_frames_count;
    const std::shared_ptr<const PatFrame> new_data;
  };

private:

  static const std::map<SignalOrigin, SignalSpec> _signal_specs;

  std::tuple<std::size_t, ullong, std::vector<double>> _eeg_signal = {};
  std::tuple<std::size_t, ullong, std::vector<double>> _ir_led_signal = {};
  std::tuple<std::size_t, ullong, std::vector<double>> _red_led_signal = {};
  std::tuple<std::size_t, ullong, std::vector<Double3d>> _accel_axes_signal = {};
  std::tuple<std::size_t, ullong, std::vector<double>> _temperature_signal = {};


  void _add_new_vector_data(SignalOrigin origin, llong timestamp, std::vector<double>& v);

  // ----------- lost frame signal hole filling

  std::function< void (EegHoleFillingArgs) >  _eeg_lost_frame_hole_filling_function = nullptr;
  std::function< void (PatHoleFillingArgs) >  _accelledstemp_lost_frame_hole_filling_function = nullptr;

  void _default_nan_filling_eeg(EegHoleFillingArgs);
  void _default_nan_filling_accelledstemp(PatHoleFillingArgs);


public:

  NeuroonSignals() {}

  // clears so far collected signals
  void clear_data();

  // sets lost frame hole filling function,
  // for particular frame
  // function should append only missing data to the given as reference vectors,
  // new data will be appended automatically.
  // it defaults to filling it with std::nan
  void set_lost_frame_hole_filling_function(std::function< void (EegHoleFillingArgs)> fun){
    _eeg_lost_frame_hole_filling_function = fun;
  }
  void set_lost_frame_hole_filling_function(std::function<void (PatHoleFillingArgs)> fun){
    _accelledstemp_lost_frame_hole_filling_function = fun;
  }

  // consumes a frame converting it to signal vectors
  void consume(std::shared_ptr<EegFrame> frame) override;
  void consume(std::shared_ptr<PatFrame> frame) override;


  virtual void setDataSourceDelegate(SinkSetDelegateKey, std::weak_ptr<IDataSourceDelegate>) override {}


  // get vectors of received samples
  const std::vector<double> & eeg_signal() const override;
  const std::vector<double> & ir_led_signal() const override;
  const std::vector<double> & red_led_signal() const override;
  const std::vector<Double3d> & accel_axes_signal() const override;
  const std::vector<double> & temperature_signal() const override;

  // ask for last sample timestamp for each signal
  ullong last_timestamp(SignalOrigin so) const override;

  // get specification of signals by its source
  static const SignalSpec & specs(SignalOrigin so){return _signal_specs.at(so);}

  // This is intended to provide information about number of already received
  // samples. The implementation may change as we might want not to store all the samples
  // in the vectors.
  std::size_t total_signal_samples(SignalOrigin ss) const override;

};

#endif
