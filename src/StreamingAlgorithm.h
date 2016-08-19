#ifndef __STREAMING_ALGORITHM__
#define __STREAMING_ALGORITHM__

#include <vector>
#include "NeuroonSignals.h"
#include "DataSink.h"

class StreamingAlgorithm{

  std::vector<DataSink*> _sinks;

protected:

  void feed_all_sinks(void*);

public:

  StreamingAlgorithm(const std::vector<DataSink*> & sinks={}) : _sinks(sinks) {}

  virtual void reset_state() = 0;
  virtual void process_input(const NeuroonSignals & input) = 0;

};

#endif
