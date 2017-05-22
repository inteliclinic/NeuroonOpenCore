#ifndef __DATA_SINK__
#define __DATA_SINK__

#include <type_traits>
#include <functional>
#include <queue>
#include <memory>
#include <iostream>

class SinkSetDelegateKey {
  friend class IDataSourceDelegate;
  SinkSetDelegateKey() {}
};

class IDataSourceDelegate {
protected:
  virtual SinkSetDelegateKey getSetSinkDelegateKey(){
    return SinkSetDelegateKey();
  }
public:
  virtual bool isDepleted() const = 0;
};

template<typename T>
class IDataSink{
public:
  // this function should be lightweight in order to not cause delays
  // in realtime streaming
  virtual void consume(T) = 0;
  virtual void setDataSourceDelegate(SinkSetDelegateKey, std::weak_ptr<IDataSourceDelegate>) = 0;
  virtual ~IDataSink(){}
};

template<typename T>
using IDataSinkSp = IDataSink<std::shared_ptr<T>>;

template<class T>
class LambdaSignalFrameDataSink : public IDataSink<T>{
  std::function<void (T)> _consume_fun = nullptr;
public:
  // consume_function should be lightweight in order to not cause delays
  // in realtime streaming
  LambdaSignalFrameDataSink(std::function<void (T)> consume_function) :
    _consume_fun(consume_function) {
  }
  void consume(T ptr) override{
    _consume_fun(std::move(ptr));
  }

  virtual void setDataSourceDelegate(SinkSetDelegateKey , std::weak_ptr<IDataSourceDelegate>) override{ return; }
};

template<typename T>
using LambdaSignalFrameDataSinkSp = LambdaSignalFrameDataSink<std::shared_ptr<T>>;

#endif
