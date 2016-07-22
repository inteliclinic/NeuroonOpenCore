#ifndef __ALGCOREDAEMON__
#define __ALGCOREDAEMON__
#include "InMsg.h"
#include <vector>
#include <memory>
#include <map>

using namespace std;

class AlgCoreDaemon{

private:
  AlgCoreDaemon() {}

  map<string, vector<InMsg> > _msg_inbox;
public:

  static AlgCoreDaemon& instance(){
    static AlgCoreDaemon inst;
    return inst;
  }

  void post_msg(string key, InMsg m){
    if ( _msg_inbox.find(key) == _msg_inbox.end() ) {
      // not found
      _msg_inbox[key] = vector<InMsg>(1, m);
    } else {
      _msg_inbox[key].push_back(m);
      // found
    }
  }

};

#endif
