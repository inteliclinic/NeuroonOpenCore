#include "StdinNeuroonFramesSource.h"

void StdinNeuroonFramesSource::_readerAction() {
  // if datasource delegate is valid

  auto sz = 1 + NeuroonSignalFrame::FrameSizeBytes;
  char buf[sz];
  while (!this->_reader_should_stop) {
    bool finish = true;

    std::size_t i = 0;
    while (i < sz) {
      std::cin.get(buf[i]);
      if (auto gc = std::cin.gcount()) {
        i++;
      } else {
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
      }
    }

    switch (buf[0]) {
    case 0: {
      if (auto sink_sp = this->_eeg_sink.lock()) {
        sink_sp->consume(std::make_shared<EegFrame>(EegFrame::from_bytes_array(
            &buf[1], NeuroonSignalFrame::FrameSizeBytes)));
        finish = false;
      }
      break;
    }
    case 1: {
      if (auto sink_sp = this->_pat_sink.lock()) {
        sink_sp->consume(std::make_shared<PatFrame>(PatFrame::from_bytes_array(
            &buf[1], NeuroonSignalFrame::FrameSizeBytes)));
        finish = false;
      }
      break;
    }
    default: {
      std::cerr << "\ninvalid frame id";
      std::fflush(stdout);
      finish = false;
      break;
    }
    }
    _reader_should_stop = _reader_should_stop || finish;
  }
  std::cerr << "reader action stopped";
  std::fflush(stdout);
}

void StdinNeuroonFramesSource::startStreaming() {
  std::cerr << "startsteaming";
  std::fflush(stdout);
  if (!this->_reader_should_stop && _reader) {
    std::cerr << "startsteaming failed";
    std::fflush(stdout);
    return;
  } else if (_reader) {
    std::cerr << "startsteaming continued";
    std::fflush(stdout);
    this->_reader_should_stop = true;
    if (_reader->joinable()) {
      _reader->join();
    }
    delete _reader;
    _reader = nullptr;
    this->_reader_should_stop = false;
  } else {
    std::cerr << "startsteaming success";
    std::fflush(stdout);
    this->_reader_should_stop = false;
    _reader = new std::thread(&StdinNeuroonFramesSource::_readerAction, this);
  }
}

void StdinNeuroonFramesSource::stopStreaming() {
  if (_reader) {
    _reader_should_stop = true;
    if (_reader->joinable()) {
      _reader->join();
    }
    delete _reader;
    _reader = nullptr;
  }
}

void StdinNeuroonFramesSource::setSink(
    std::weak_ptr<IDataSink<std::shared_ptr<PatFrame>>> sink) {
  _pat_sink = std::static_pointer_cast<IDataSinkSp<PatFrame>>(sink.lock());
  if (auto sinksp = _pat_sink.lock()) {
    sinksp->setDataSourceDelegate(
        this->IPushingDataSourceSp<PatFrame>::getSetSinkDelegateKey(),
        std::dynamic_pointer_cast<IDataSourceDelegate>(
            this->shared_from_this()));
  }
}

void StdinNeuroonFramesSource::setSink(
    std::weak_ptr<IDataSink<std::shared_ptr<EegFrame>>> sink) {
  _eeg_sink = std::static_pointer_cast<IDataSinkSp<EegFrame>>(sink.lock());
  if (auto sinksp = _eeg_sink.lock()) {
    sinksp->setDataSourceDelegate(
        this->IPushingDataSourceSp<EegFrame>::getSetSinkDelegateKey(),
        std::dynamic_pointer_cast<IDataSourceDelegate>(
            this->shared_from_this()));
  }
}
