#ifndef __DATA_SINK__
#define __DATA_SINK__

#include "Signal.h"
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
 public:
  // consume_function should be lightweight in order to not cause delays
  // in realtime streaming
  SignalFrameDataSink() : DataSink () {}

  virtual void consume(SignalFrame* sfr) = 0;

  void consume(void* sfp) override{
    consume((SignalFrame*)sfp);
  }
};


class LambdaSignalFrameDataSink : public SignalFrameDataSink{
  std::function<void (SignalFrame*)> _frame_consume_fun = nullptr;
 public:
  // consume_function should be lightweight in order to not cause delays
  // in realtime streaming
  LambdaSignalFrameDataSink(std::function<void (SignalFrame*)> consume_function) :
  _frame_consume_fun(consume_function) {}

  void consume(SignalFrame* ptr) override{
    _frame_consume_fun(ptr);
  }
};


#endif
