#ifndef __NEUROON_SIGNAL_FRAMES__
#define __NEUROON_SIGNAL_FRAMES__

#include "CommonTypes.h"
#include <cstdint>
#include <vector>
#include <stdexcept>

struct NeuroonFrameBytes{
  enum class SourceStream { EEG, ALT };
  enum class ByteOrder { BE, LE };
  static const ByteOrder DefaultByteOrder = ByteOrder::LE;
  SourceStream source_stream;
  char* bytes;
  std::size_t size;
};


struct NeuroonSignalFrame{
  static const uint FrameSizeBytes = 20;
  std::uint32_t timestamp;
  virtual ~NeuroonSignalFrame() = 0;
};

inline NeuroonSignalFrame::~NeuroonSignalFrame() {}


struct EegFrame : public NeuroonSignalFrame{
  static const uint DefaultEmissionInterval_ms = 64;
  static const std::size_t Length = 8;

  static EegFrame from_bytes_array(const char*, std::size_t, NeuroonFrameBytes::ByteOrder bo=NeuroonFrameBytes::DefaultByteOrder);
  std::int16_t signal[Length];
};

struct AccelAxes{
  std::int16_t x, y, z;
};

struct AccelLedsTempFrame : public NeuroonSignalFrame{
  static const uint DefaultEmissionInterval_ms = 40;
  static AccelLedsTempFrame from_bytes_array(const char* bytes, std::size_t, NeuroonFrameBytes::ByteOrder bo=NeuroonFrameBytes::DefaultByteOrder);
  std::int32_t ir_led;
  std::int32_t red_led;
  AccelAxes accel_axes;
  std::int8_t temperature[2];
};


template<class T>
class IPullBasedFrameSource{

public:
  virtual std::vector<T>& get_frames() = 0;
};


#endif
