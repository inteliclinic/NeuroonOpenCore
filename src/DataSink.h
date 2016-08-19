#ifndef __DATA_SINK__
#define __DATA_SINK__

#include "NeuroonSignals.h"
#include <functional>

class DataSink{
public:
  // this function should be lightweight in order to not cause delays
  // in realtime streaming
  virtual void consume(void*) = 0;
};


// consumes SignalFrame pointer
// WARNING use it only if you know that passed pointer points
// to SignalFrame!
class SignalFrameDataSink : public DataSink{
  std::function<void (SignalFrame*)> _frame_consume_fun;
 public:
  // consume_function should be lightweight in order to not cause delays
  // in realtime streaming
  SignalFrameDataSink(std::function<void (SignalFrame*)> consume_function) :
    _frame_consume_fun(consume_function) {}

  virtual void consume(void*) override;
};

#endif
