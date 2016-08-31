// #include <random>
#include "EegQualityStream.h"

void EegQualityStream::process_input(const INeuroonSignals & ns){
  // TODO: now is dummy implementation

  // number of windows for computing signal quality value
  auto windows_count = (ns.total_signal_samples(SignalOrigin::EEG) - _last_counter) / (_window_size - _overlap);

  for(std::size_t i=0; i<windows_count; i++){
    // every full window
    auto start_iterator = ns.eeg_signal().begin() + _last_counter;
    auto r = _compute_quality({start_iterator, start_iterator+_window_size});
    feed_all_sinks(r);
    // move counter by processed samples count
    _last_counter += _window_size - _overlap;
  }

}

EegQuality EegQualityStream::_compute_quality(VectorView<double>&&){
  // TODO
  return (EegQuality)(std::rand() % 4);
}

void EegQualityStream::reset_state(){
  _last_counter = 0;
}
