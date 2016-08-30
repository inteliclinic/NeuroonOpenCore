#ifndef __ALGCOREDAEMON__
#define __ALGCOREDAEMON__

// #include "InValue.h"
#include <vector>
#include <memory>
#include <map>
#include "DataSink.h"
#include "StreamingAlgorithm.h"
#include "NeuroonSignals.h"

// daemon managing signal processing and algorithm execution
class AlgCoreDaemon : public IDataSink<NeuroonFrameBytes>, public IDataSink<EegFrame>, public IDataSink<AccelLedsTempFrame>{

private:

  bool _processing_in_progress = false;
  // std::map<std::string, std::vector<InValue> > _msg_inbox;

  // algorithms working with stream of data coming from mask
  std::vector<std::unique_ptr<IStreamingAlgorithm>> _stream_algorithms = {};

  // module aggregating input data frames and converting in to
  // a continuous signal
  NeuroonSignals _neuroon_signals;

  // it "wakes" up streaming algorithms by sending to them actual state
  // of neuroon signals
  void _make_streaming_algorithms_step();

  void _add_streaming_algorithms(std::unique_ptr<IStreamingAlgorithm> & saup, bool suppress_warning);
public:

  AlgCoreDaemon() {}

  // can't copy as it contains vectors of unique_pointers
  AlgCoreDaemon(const AlgCoreDaemon &) = delete;
  AlgCoreDaemon & operator=(const AlgCoreDaemon &) = delete;

  // call it after adding algorithm and before starting receiving frames
  void start_processing();

  // call it to finalize receiving frames.
  void end_processing();

  // Receive a frame of signal
  void consume(NeuroonFrameBytes& frame_stream) override;
  void consume(EegFrame& frame) override;
  void consume(AccelLedsTempFrame& frame) override;

  // for now it isnt possible to remove algorithm from daemon
  void add_streaming_algorithms(std::unique_ptr<IStreamingAlgorithm> & saup);
  void add_streaming_algorithms(std::vector<std::unique_ptr<IStreamingAlgorithm>> saups);


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

