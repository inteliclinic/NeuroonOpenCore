#ifndef __ALGCOREDAEMON__
#define __ALGCOREDAEMON__

// #include "InValue.h"
#include <vector>
#include <memory>
#include <map>
#include "StreamingAlgorithm.h"
#include "NeuroonSignals.h"

// daemon managing signal processing and algorithm execution
// for now intended to be used as a singleton
class AlgCoreDaemon{

private:
  AlgCoreDaemon() {}

  // std::map<std::string, std::vector<InValue> > _msg_inbox;

  // algorithms working with stream of data coming from mask
  std::vector<std::unique_ptr<StreamingAlgorithm>> _stream_algorithms = {};

  // module aggregating input data frames and converting in to
  // a continuous signal
  NeuroonSignals _neuroon_signals;


  // it "wakes" up streaming algorithms by sending to them actual state
  // of neuroon signals
  void _make_streaming_algorithms_step();

public:

  // Singleton
  static AlgCoreDaemon& instance(){
    static AlgCoreDaemon inst;
    return inst;
  }

  // Name explains it.
  // In the future it may be wrapped with methods such as: start_processing
  // or sthing like this.
  void reset_algorithms_state();

  // Receive a frame of signal from Neuron mask.
  void receive_stream_frame(SignalFrame * sf, bool streaming_algs_step=true);


  // // ----- in case we need some message receiving by the daemon
  // void post_msg(std::string key, InValue m){
  //   if ( _msg_inbox.find(key) == _msg_inbox.end() ) {
  //     // not found
  //     _msg_inbox[key] = std::vector<InValue>(1, m);
  //   } else {
  //     _msg_inbox[key].push_back(m);
  //     // found
  //   }
  // }

};

#endif
