#include "RollingAlgorithms.h"
#include <vector>

using namespace std;

void RollingApply::init(size_t n, const RollWindow &window ) {}

double RollingApply::step(const vector<double> &v, StepType type){
  return _apply_fun(v);
}

