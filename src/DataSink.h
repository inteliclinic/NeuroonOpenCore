#ifndef __DATA_SINK__
#define __DATA_SINK__

#include <type_traits>
#include <functional>
#include "SignalTypes.h"
#include "NeuroonSignalFrames.h"

template<typename T>
class IDataSink{
public:
  // this function should be lightweight in order to not cause delays
  // in realtime streaming
  virtual void consume(T&) = 0;
};


template<class T>
class LambdaSignalFrameDataSink : public IDataSink<T>{
  std::function<void (T&)> _consume_fun = nullptr;
 public:
  // consume_function should be lightweight in order to not cause delays
  // in realtime streaming
  LambdaSignalFrameDataSink(std::function<void (T&)> consume_function) :
  _consume_fun(consume_function) {
  }
  void consume(T& ptr) override{
    _consume_fun(ptr);
  }
};


#endif
