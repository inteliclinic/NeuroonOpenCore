#ifndef __STDIN_NEUROON_FRAMES_SOURCE__
#define __STDIN_NEUROON_FRAMES_SOURCE__

#include <cstring>
#include <iostream>
#include <memory>
#include <atomic>
#include <thread>
#include "DataSink.h"
#include "DataSource.h"

#include "NeuroonSignalFrames.h"

class StdinNeuroonFramesSource : public IPushingDataSourceSp<EegFrame>,
                                 public IPushingDataSourceSp<PatFrame>,
                                 public std::enable_shared_from_this<StdinNeuroonFramesSource>{

private:
  std::weak_ptr<IDataSinkSp<EegFrame>> _eeg_sink;
  std::weak_ptr<IDataSinkSp<PatFrame>> _pat_sink;
  std::thread *_reader = nullptr;
  std::atomic<bool> _reader_should_stop;

  void _readerAction();

public:
  StdinNeuroonFramesSource() : _reader_should_stop(false) {}

  virtual ~StdinNeuroonFramesSource() { this->stopStreaming(); }

  void startStreaming() override;

  void stopStreaming() override;

  virtual void setSink(std::weak_ptr<IDataSink<std::shared_ptr<PatFrame>>> sink) override;
  virtual void setSink(std::weak_ptr<IDataSink<std::shared_ptr<EegFrame>>> sink) override;

  bool isDepleted() const override { return false; }
};

#endif
