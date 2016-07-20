#ifndef __IRBALGORITHM__
#define __IRBALGORITHM__

#define __ROLLING__

#include <vector>
#include <cstdlib>

class RollWindow{

public:
  enum class Type{LEFT,CENTER,RIGHT};

private:
  Type _type;
  size_t _length;

public:
  RollWindow(size_t length, Type type=Type::LEFT):_type(type), _length(length){}

  Type type() const { return _type; }
  size_t length() const { return _length; }

};


class IRbAlgorithm{
public:

  enum class StepType{START,STEP,END};
  virtual void init(size_t n, const RollWindow &window ) = 0;
  virtual double step(const std::vector<double> &v, StepType type) = 0;

};

#endif
