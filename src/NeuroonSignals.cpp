#include "NeuroonSignals.h"

const std::map<SignalOrigin, SignalSpec> NeuroonSignals::_signal_specs = {
  {SignalOrigin::EEG,           SignalSpec(SignalOrigin::EEG, 125)},
  {SignalOrigin::IR_LED,        SignalSpec(SignalOrigin::IR_LED, 25)},
  {SignalOrigin::ACCELEROMETER, SignalSpec(SignalOrigin::ACCELEROMETER, 25)},
  {SignalOrigin::TEMPERATURE,   SignalSpec(SignalOrigin::TEMPERATURE, 1)}
};
