#include "AlgCoreDaemon.h"
#include "logger.h"


void AlgCoreDaemon::_make_streaming_algorithms_step(){
  LOG(DEBUG) << "Streaming algorithms step";
  for(auto & alg : _stream_algorithms){
	  LOG(DEBUG) << "stepping...";
    alg->process_input(_neuroon_signals);
  }
}


void AlgCoreDaemon::end_processing(){
  for(auto & alg : _stream_algorithms){
    alg->end_streaming(_neuroon_signals);
  }
	LOG(INFO) << "Unsetting processing in progress flag";
  _processing_in_progress = false;
}


void AlgCoreDaemon::start_processing(){

	LOG(INFO) << "Reseting streaming algorithms state.";
  for(auto & alg : _stream_algorithms){
    alg->reset_state();
  }

	LOG(INFO) << "Clearing signal data.";
  _neuroon_signals.clear_data();

	LOG(INFO) << "Setting processing in progress flag";
  _processing_in_progress = true;
}


void AlgCoreDaemon::consume (NeuroonFrameBytes& frame_stream) {
  if (!_processing_in_progress) {
    LOG(WARNING) << "Unintended behaviour: Consuming data when processing flag turned off.";
  }
  switch(frame_stream.source_stream){
  case NeuroonFrameBytes::SourceStream::EEG:{
    auto f = EegFrame::from_bytes_array(frame_stream.bytes, frame_stream.size);
    consume(f);
    break;
  }
  case NeuroonFrameBytes::SourceStream::ALT:{
    auto f = PatFrame::from_bytes_array(frame_stream.bytes, frame_stream.size);
    consume(f);
    break;
  }
  }

  _make_streaming_algorithms_step();
}


void AlgCoreDaemon::consume (PatFrame& frame) {
  if (!_processing_in_progress) {
    LOG(WARNING) << "Unintended behaviour: Consuming data when processing flag turned off.";
  }
  // pass aggregating staff to the neuroonsignals instance
  _neuroon_signals.consume(frame);
}

void AlgCoreDaemon::consume (EegFrame& frame) {
  if (!_processing_in_progress) {
    LOG(WARNING) << "Unintended behaviour: Consuming data when processing flag turned off.";
  }
  // pass aggregating staff to the neuroonsignals instance
  _neuroon_signals.consume(frame);
}


void AlgCoreDaemon::_add_streaming_algorithms(std::unique_ptr<IStreamingAlgorithm> & saup, bool suppress_warning){
  if (!suppress_warning && _processing_in_progress) {
    LOG(WARNING) << "Algorithm added when processing in progress flag is set!";
  }
  _stream_algorithms.push_back(std::move(saup));
}

void AlgCoreDaemon::add_streaming_algorithms(std::unique_ptr<IStreamingAlgorithm> & saup){
  _add_streaming_algorithms(saup, false);
}


void AlgCoreDaemon::add_streaming_algorithms(std::vector<std::unique_ptr<IStreamingAlgorithm>> saups) {
  if (_processing_in_progress) {
    LOG(WARNING) << "Algorithms added when processing in progress flag is set!";
  }
  for(auto & saup : saups) _add_streaming_algorithms(saup, true);
}
