#ifndef __ROLLING__
#define __ROLLING__

#include <vector>
#include <memory>
#include "IRbAlgorithm.h"

using namespace std;

class Rolling{

  shared_ptr<const vector<double> > _v;
  RollWindow _window;

public:

  Rolling(shared_ptr<const vector<double> > v, size_t window_length) :
    _v(v), _window(RollWindow(window_length)) {}
  Rolling(shared_ptr<const vector<double> > v, const RollWindow &window) :
    _v(v), _window(window) {}

  vector<double> run_algorithm(IRbAlgorithm & );

  vector<double> sum();
  vector<double> mean();
  vector<double> min();
  vector<double> max();
  vector<double> apply(std::function<double (const vector<double> &) > f);
};

#endif
