#ifndef __ROLLINGALGORITHMS__
#define __ROLLINGALGORITHMS__

#include "IRbAlgorithm.h"
#include <functional>
#include <vector>
#include <set>

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

  function<bool (double, double)> _cmp_fun = nullptr;
  set<double, function<bool (double, double)> > _set;
  double _prev_last;
  size_t _windows_length;

public:

  RollingPriority(function<bool (double, double)> cmp_fun) :
    _cmp_fun(cmp_fun), _set(set<double, function<bool (double, double)> >(cmp_fun)) {}

  void init(size_t n, const RollWindow &window ) override;
  double step(const vector<double> &v, StepType type) override;

};
#endif
