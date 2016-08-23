#include "AlgCoreDaemon.h"


void AlgCoreDaemon::_make_streaming_algorithms_step(){
  for(auto & alg : _stream_algorithms){
    alg->process_input(_neuroon_signals);
  }
}

void AlgCoreDaemon::reset_algorithms_state(){
  for(auto & alg : _stream_algorithms){
    alg->reset_state();
  }
}

void AlgCoreDaemon::receive_stream_frame(SignalFrame * sf, bool streaming_algs_step){
  // pass aggregating staff to the neuroonsignals instance
  _neuroon_signals.receive_frame(sf);

  // run stream algs with newly aquired data
  if(streaming_algs_step){
    _make_streaming_algorithms_step();
  }
}

void AlgCoreDaemon::add_streaming_algorithms(std::unique_ptr<IStreamingAlgorithm> & saup){
  _stream_algorithms.push_back(std::move(saup));
}

void AlgCoreDaemon::add_streaming_algorithms(std::vector<std::unique_ptr<IStreamingAlgorithm>> saups) {
  for(auto & saup : saups) add_streaming_algorithms(saup);
}
