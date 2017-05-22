#ifndef __FRAME_STREAM_PIPE__
#define __FRAME_STREAM_PIPE__

#include "CommonTypes.h"
#include "DataSink.h"
#include "DataSource.h"
#include "IStreamPipe.h"
#include "NeuroonSignalFrames.h"
#include "logger.h"

#include <iostream>
#include <memory>

class IFrameStreamPipe : public IStreamPipe {
public:
  virtual ~IFrameStreamPipe() {}
  virtual bool passNextFrameWithTimestamp(ullong timestamp) = 0;
};

// [TODO] what about removing restrinction on shared_ptr in piping data
template <class T> class FrameStreamPipe : public IFrameStreamPipe {

  ullong _frame_transmitted = 0;
  std::weak_ptr<IPullingDataSourceSp<T>> _source;
  std::weak_ptr<IDataSinkSp<T>> _sink;

  bool _depleted = false;

  bool _passNextFrame(bool change_timestamp, ullong timestamp = 0) {
    bool just_depleted = false;
    bool success = false;
    if (!this->isDepleted() && !this->isBroken()) {
      if (auto ssource = _source.lock()) {
        if (auto ssink = _sink.lock()) {
          if (auto frame = ssource->getNextValue()) {
            if (change_timestamp) {
              std::static_pointer_cast<NeuroonSignalFrame>(frame)->timestamp =
                  timestamp;
            }
            ssink->consume(frame);
            success = true;
            _frame_transmitted++;
          }
          if (ssource->isDepleted()) {
            _depleted = true;
            just_depleted = true;
          }
        } else {
          LOG(WARNING) << "Pipe passing frame: sink shared pointer empty";
        }
      } else {
        LOG(WARNING) << "Pipe passing frame: source shared pointer empty";
      }
    } else {
      if (this->isDepleted()) {
        LOG(DEBUG) << "Source depleted";
      } else {
        LOG(WARNING) << "Pipe broken: shared pointer expired";
      }
    }
    _log_warnings(just_depleted);
    return success;
  }

  void _log_warnings(bool just_depleted = false) {
    if (_sink.expired()) {
      // LOG(WARNING) << "Pipe broken. Sink pointer is expired.";
    }
    if (_source.expired()) {
      // LOG(WARNING) << "Pipe broken. Source pointer is expired.";
    }
    if (!just_depleted && _depleted) {
      // LOG(INFO) << "Pipe depleted. Nothing is transmitted.";
    }
  }

public:
  FrameStreamPipe(std::weak_ptr<IPullingDataSourceSp<T>> source,
                  std::weak_ptr<IDataSinkSp<T>> sink)
      : _source(source), _sink(sink), _frame_transmitted(0) {
    static_assert(std::is_base_of<NeuroonSignalFrame, T>::value,
                  "T must inherit from NeuroonSignalFrame");
    if (auto ssource = _source.lock()) {
      if (auto ssink = _sink.lock()) {
        if (ssource->isDepleted()) {
          LOG(WARNING) << "Creating pipe with depleted source.";
          _depleted = true;
        }
      }
    }
    _log_warnings();
  }

  // passes ith frame between source and sink
  bool passNextFrame() override { _passNextFrame(false); }

  // passes ith frame between source and sink and sets its timestamp
  bool passNextFrameWithTimestamp(ullong timestamp) override {
    return _passNextFrame(true, timestamp);
  }

  bool isBroken() const override {
    return _sink.expired() || _source.expired();
  }
  bool isDepleted() const { return _depleted; }
};

#endif
