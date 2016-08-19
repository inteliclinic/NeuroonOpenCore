#include "StreamingAlgorithm.h"

void StreamingAlgorithm::feed_all_sinks(void* result){
  for(auto & s : _sinks){
    if(s != nullptr){
      s->consume(result);
    }
  }
}
