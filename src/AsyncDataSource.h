#ifndef __ASYNC_DATA_SOURCE__
#define __ASYNC_DATA_SOURCE__

#include <atomic>
#include <iostream>
#include <thread>

#include "DataSource.h"

template <class T>
class AsyncDataSource
    : public IPushingDataSourceSp<T>,
      public std::enable_shared_from_this<AsyncDataSource<T>> {
private:
  std::weak_ptr<IDataSinkSp<T>> _sink;
  std::thread *_reader = nullptr;
  std::atomic<bool> _reader_should_stop;

  void _readerAction() {
    // if datasource delegate is valid
    if (auto sink_sp = this->_sink.lock()) {
      std::shared_ptr<T> val = nullptr;
      do {
        // reader got a value to be passed
        if (val) {
          sink_sp->consume(std::move(val));
        }
        // stopping mechanism
        if (this->_reader_should_stop) {
          std::cerr << "stopping.";
          std::fflush(stdout);
          break;
        }
        // get the value from async
        val = std::move(this->readValue());
      } while (true);
    }
  }

public:
  AsyncDataSource() : _reader_should_stop(false) {}

  virtual ~AsyncDataSource() { this->stopStreaming(); }

  void startStreaming() override {
    if (!this->_reader_should_stop && _reader) {
      return;
    } else if (_reader) {
      this->_reader_should_stop = true;
      if (_reader->joinable()) {
        _reader->join();
      }
      delete _reader;
      _reader = nullptr;
      this->_reader_should_stop = false;
    } else {
      this->_reader_should_stop = false;
      _reader = new std::thread(&AsyncDataSource::_readerAction, this);
    }
  }

  void stopStreaming() override {
    if (_reader) {
      _reader_should_stop = true;
      if (_reader->joinable()) {
        _reader->join();
      }
      delete _reader;
      _reader = nullptr;
    }
  }

  virtual void
  setSink(std::weak_ptr<IDataSink<std::shared_ptr<T>>> sink) override {
    _sink = std::static_pointer_cast<IDataSinkSp<T>>(sink.lock());
    if (auto sinksp = _sink.lock()) {
      sinksp->setDataSourceDelegate(this->getSetSinkDelegateKey(),
                                    this->shared_from_this());
    }
  }

  virtual bool isDepleted() const override = 0;

protected:
  virtual std::shared_ptr<T> readValue() = 0;
};

#endif
