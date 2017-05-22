#ifndef __STREAM_QUEUE__
#define __STREAM_QUEUE__

#include "DataSink.h"
#include "DataSource.h"
#include <iostream>

template <typename T>
class StreamQueue : public IDataSinkSp<T>, public IPullingDataSourceSp<T> {

private:
  std::queue<std::shared_ptr<T>> _q;
  std::weak_ptr<IDataSourceDelegate> _source_delegate;

public:
  virtual ~StreamQueue() {}

  // --------- sink interface
  virtual void consume(std::shared_ptr<T> t) override {
    _q.push(std::move(t));
  }

  virtual void
  setDataSourceDelegate(SinkSetDelegateKey, std::weak_ptr<IDataSourceDelegate> delegate) override {
    std::fflush(stdout);
    _source_delegate = delegate;
  }

  // --------- source interface

  virtual std::shared_ptr<T> getNextValue() override {
    if(!_q.size()) return nullptr;
    return std::move(this->pop());
  }

  virtual bool isDepleted() const override {
    if (auto delsp = _source_delegate.lock()) {
      return delsp->isDepleted();
    }
    std::cerr <<"damn..";
    std::fflush(stdout);
    return true;
  }

  // --------- queue delegate

  virtual std::shared_ptr<T> peek() const { return _q.front(); }

  virtual std::shared_ptr<T> pop() {
    auto t = _q.front();
    _q.pop();
    return t;
  }

  virtual std::size_t size() const { return _q.size(); }

  virtual void clear() {
    while (_q.size()) {
      _q.pop();
    }
  }
};

#endif
