#ifndef __ROLLINGALGORITHMS__
#define __ROLLINGALGORITHMS__

#include "IRbAlgorithm.h"
#include <functional>
#include <vector>
#include <set>


class RollingApply: public IRbAlgorithm{

  std::function<double (const std::vector<double> &)> _apply_fun = nullptr;


public:

  RollingApply(std::function<double (const std::vector<double> &)> apply_fun) :
    _apply_fun(apply_fun) {}

  void init(size_t n, const RollWindow &window ) override;
  double step(const std::vector<double> &v, StepType type) override;

};

class RollingPriority: public IRbAlgorithm{

  std::function<bool (double, double)> _cmp_fun = nullptr;
  std::set<double, std::function<bool (double, double)> > _set;
  double _prev_last;
  size_t _windows_length;

 public:

 RollingPriority(std::function<bool (double, double)> cmp_fun) :
  _cmp_fun(cmp_fun), _set(std::set<double, std::function<bool (double, double)> >(cmp_fun)) {}

  void init(size_t n, const RollWindow &window ) override;
  double step(const std::vector<double> &v, StepType type) override;

};

class RollingSumOrMean: public IRbAlgorithm{
public:
  enum class Type{SUM,MEAN};

  double _sum=0;
  size_t _prev_win_length;
  double _prev_last;
  Type _type;

public:
  RollingSumOrMean(Type sum_or_mean=Type::SUM) : _type(sum_or_mean) {}
  void init(size_t n, const RollWindow &window ) override;
  double step(const std::vector<double> &v, StepType type) override;

};
#endif
