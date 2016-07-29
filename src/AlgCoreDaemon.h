#ifndef __ALGCOREDAEMON__
#define __ALGCOREDAEMON__

#include "InValue.h"
#include <vector>
#include <memory>
#include <map>

class AlgCoreDaemon{

private:
  AlgCoreDaemon() {}

  std::map<std::string, std::vector<InValue> > _msg_inbox;
public:

  static AlgCoreDaemon& instance(){
    static AlgCoreDaemon inst;
    return inst;
  }

  void post_msg(std::string key, InValue m){
    if ( _msg_inbox.find(key) == _msg_inbox.end() ) {
      // not found
      _msg_inbox[key] = std::vector<InValue>(1, m);
    } else {
      _msg_inbox[key].push_back(m);
      // found
    }
  }

};

#endif
