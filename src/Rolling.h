#ifndef __ROLLING__
#define __ROLLING__

#include <vector>
#include <memory>
#include "IRbAlgorithm.h"

class Rolling{

  std::shared_ptr<const std::vector<double> > _v;
  RollWindow _window;

public:

  Rolling(std::shared_ptr<const std::vector<double> > v, size_t window_length) :
    _v(v), _window(RollWindow(window_length)) {}
  Rolling(std::shared_ptr<const std::vector<double> > v, const RollWindow &window) :
    _v(v), _window(window) {}

  std::vector<double> run_algorithm(IRbAlgorithm & );

  std::vector<double> sum();
  std::vector<double> mean();
  std::vector<double> min();
  std::vector<double> max();
  std::vector<double> priority(std::function<bool (double, double) > f);
  std::vector<double> apply(std::function<double (const std::vector<double> &) > f);
};

#endif
