#ifndef __NEUROON_SIGNAL_FRAMES__
#define __NEUROON_SIGNAL_FRAMES__

#include "CommonTypes.h"
#include <cstdint>
#include <vector>


struct NeuroonSignalFrame{
  ullong timestamp;
  virtual ~NeuroonSignalFrame() = 0;
};
inline NeuroonSignalFrame::~NeuroonSignalFrame() {}


struct EegFrame : public NeuroonSignalFrame{
  static const uint DefaultEmissionInterval_ms = 64;
  static const std::size_t Length = 8;
  std::int16_t signal[Length];
};

struct AccelAxes{
  std::int16_t x, y, z;
};

struct AccelLedsTempFrame : public NeuroonSignalFrame{
  static const uint DefaultEmissionInterval_ms = 40;
  AccelAxes accel_axes;
  std::int16_t ir_led;
  std::int16_t red_led;
  std::int32_t temperature;
};


template<class T>
class IPullBasedFrameSource{

public:
  virtual std::vector<T>& get_frames() = 0;
};


#endif
