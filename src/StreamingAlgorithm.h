#ifndef __STREAMING_ALGORITHM__
#define __STREAMING_ALGORITHM__

#include <vector>
#include "NeuroonSignals.h"
#include "DataSink.h"

class IStreamingAlgorithm{
public:
  virtual void reset_state () = 0;
  virtual void process_input (const NeuroonSignals & input) = 0;
};

template<typename T>
class SinkStreamingAlgorithm : IStreamingAlgorithm {

  typedef DataSink<T> StreamSink;
  std::vector< StreamSink* > _sinks;

protected:

  void feed_all_sinks(T result){
    for(auto & s : _sinks){
      if(s != nullptr){
        s->consume(result);
      }
    }
  }

public:

  SinkStreamingAlgorithm (const std::vector< StreamSink* > & sinks={}) : _sinks(sinks) {}

};

#endif
