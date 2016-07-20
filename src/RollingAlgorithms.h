#ifndef __ROLLINGALGORITHMS__
#define __ROLLINGALGORITHMS__

#include "IRbAlgorithm.h"
#include <functional>
#include <vector>
#include <queue>

using namespace std;

class RollingApply: public IRbAlgorithm{

  function<double (const vector<double> &)> _apply_fun = nullptr;


public:

  RollingApply(function<double (const vector<double> &)> apply_fun) :
    _apply_fun(apply_fun) {}

  void init(size_t n, const RollWindow &window ) override;
  double step(const vector<double> &v, StepType type) override;

};

class RollingPriority: public IRbAlgorithm{

  function<int (double, double)> _cmp_fun = nullptr;


public:

  RollingPriority(function<int (double, double)> cmp_fun) :
    _cmp_fun(cmp_fun) {}

  void init(size_t n, const RollWindow &window ) override;
  double step(const vector<double> &v, StepType type) override;

};
#endif
