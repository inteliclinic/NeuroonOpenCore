#ifndef __FRAME_STREAM_PIPE__
#define __FRAME_STREAM_PIPE__

#include "CommonTypes.h"
#include "DataSource.h"
#include "NeuroonSignalFrames.h"
#include "DataSink.h"
#include "logger.h"

#include <memory>

class IFrameStreamPipe{
public:
  virtual void pass_next_frame() = 0;
  virtual void pass_next_frame_with_timestamp(ullong timestamp) = 0;
  virtual bool is_broken() const = 0;
  virtual bool is_exhausted() const = 0;
};

template<class T>
class FrameStreamPipe : public IFrameStreamPipe{

  ullong _frame_transmitted = 0;
  std::weak_ptr<IPullBasedOfflineSource<T>> _source;
  std::weak_ptr<IDataSink<T>> _sink;

  bool _exhausted = false;

  void _pass_next_frame(bool change_timestamp, ullong timestamp=0) {
    bool just_exhausted = false;
    if(!is_exhausted() && !is_broken()){
      if (auto ssource = _source.lock()) {
        if (auto ssink = _sink.lock()) {
          auto& frame = ssource->get_values()[_frame_transmitted];
          if(change_timestamp){
            ((NeuroonSignalFrame*)&frame)->timestamp = timestamp;
          }
          ssink->consume(frame);
          _frame_transmitted++;
          if (_frame_transmitted >= ssource->get_values().size()) {
            _exhausted = true;
            just_exhausted = true;
          }
        }
      }
    }
    _log_warnings(just_exhausted);
  }

  void _log_warnings(bool just_exhausted=false){
    if(_sink.expired()){
      LOG(WARNING) << "Pipe broken. Sink pointer is expired.";
    }
    if(_source.expired()){
      LOG(WARNING) << "Pipe broken. Source pointer is expired.";
    }
    if (!just_exhausted && _exhausted) {
      LOG(INFO) << "Pipe exhausted. Nothing is transmitted.";
    }
  }

 public:

 FrameStreamPipe(std::weak_ptr<IPullBasedOfflineSource<T>> source,
                 std::weak_ptr<IDataSink<T>> sink) :
   _source(source), _sink(sink), _frame_transmitted(0) {
    static_assert(std::is_base_of<NeuroonSignalFrame, T>::value, "T must inherit from NeuroonSignalFrame");
    if (auto ssource = _source.lock()) {
      if (auto ssink = _sink.lock()) {
        if (_frame_transmitted >= ssource->get_values().size()) {
          _exhausted = true;
        }
      }
    }
    _log_warnings();
  }

  // passes ith frame between source and sink
  void pass_next_frame() override {
    _pass_next_frame(false);
  }

  // passes ith frame between source and sink and sets its timestamp
  void pass_next_frame_with_timestamp(ullong timestamp) override {
    _pass_next_frame(true, timestamp);
  }

  bool is_broken() const override { return _sink.expired() || _source.expired(); }
  bool is_exhausted() const override { return _exhausted; }

};


#endif
