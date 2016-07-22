#include "RollingAlgorithms.h"
#include <vector>

using namespace std;

void RollingApply::init(size_t n, const RollWindow &window ) {}

double RollingApply::step(const vector<double> &v, StepType type){
  return _apply_fun(v);
}


void RollingPriority::init(size_t n, const RollWindow &window ) {
  _windows_length = window.length();
}


double RollingPriority::step(const vector<double> &v, StepType type){
  switch(type){
  case StepType::START:
    _set.insert(v.begin(),v.end());
    _prev_last = *(v.begin());
    break;
  case StepType::END:
  case StepType::STEP:
    if(_set.size()==_windows_length){
      _set.erase(_prev_last);
      _prev_last = *(v.begin());
    }
    _set.insert(*(v.end()-1));
    break;
  }
  return *_set.begin();
}
