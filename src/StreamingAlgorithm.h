#ifndef __STREAMING_ALGORITHM__
#define __STREAMING_ALGORITHM__

#include <vector>
#include "NeuroonSignals.h"
#include "DataSink.h"

class IStreamingAlgorithm{
public:
  virtual ~IStreamingAlgorithm(){}
  virtual void reset_state () = 0;
  virtual void process_input (const INeuroonSignals & input ) = 0;
  virtual void end_streaming (const INeuroonSignals & input) = 0;
};


template<typename T>
class SinkStreamingAlgorithm : public IStreamingAlgorithm {

  std::vector< IDataSink<T>* > _sinks;

protected:

  void feed_all_sinks(T result){
    for(auto & s : _sinks){
      if(s != nullptr){
        s->consume(result);
      }
    }
  }

public:

  SinkStreamingAlgorithm (const std::vector< IDataSink<T>* > & sinks={}) : _sinks(sinks) {}

};

template<class T>
using SinkStreamingAlgorithmSp = SinkStreamingAlgorithm<std::shared_ptr<T>>;

#endif
