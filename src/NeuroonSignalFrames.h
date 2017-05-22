#ifndef __NEUROON_SIGNAL_FRAMES__
#define __NEUROON_SIGNAL_FRAMES__

#include "CommonTypes.h"
#include <cstdint>
#include <stdexcept>
#include <vector>

struct NeuroonFrameBytes {
  enum class SourceStream { EEG, ALT };
  enum class ByteOrder { BE, LE };
  static const ByteOrder DefaultByteOrder = ByteOrder::LE;
  SourceStream source_stream;
  char *bytes;
  std::size_t size;
};

struct NeuroonSignalFrame {
  static const uint FrameSizeBytes = 20;
  std::uint32_t timestamp;

  // pass allocated memory of size NeuroonSignalFrame::FrameSizeBytes
  virtual void to_bytes(char *,
                        NeuroonFrameBytes::ByteOrder =
                            NeuroonFrameBytes::DefaultByteOrder) const = 0;
  virtual ~NeuroonSignalFrame() = 0;
};

// must be implemented for valid compilation
inline NeuroonSignalFrame::~NeuroonSignalFrame() {}

struct EegFrame : public NeuroonSignalFrame {
  static const uint DefaultEmissionInterval_ms = 64;
  static const std::size_t Length = 8;

  // pass allocated memory of size NeuroonSignalFrame::FrameSizeBytes
  void to_bytes(char *,
                NeuroonFrameBytes::ByteOrder =
                    NeuroonFrameBytes::DefaultByteOrder) const override;
  static EegFrame from_bytes_array(
      const char *, std::size_t,
      NeuroonFrameBytes::ByteOrder bo = NeuroonFrameBytes::DefaultByteOrder);
  std::int16_t signal[Length];
};

struct AccelAxes {
  std::int16_t x, y, z;
};

struct PatFrame : public NeuroonSignalFrame{
  static const uint DefaultEmissionInterval_ms = 40;
  static PatFrame from_bytes_array(const char* bytes, std::size_t, NeuroonFrameBytes::ByteOrder bo=NeuroonFrameBytes::DefaultByteOrder);
  void to_bytes(char*, NeuroonFrameBytes::ByteOrder=NeuroonFrameBytes::DefaultByteOrder) const override;
  std::int32_t ir_led;
  std::int32_t red_led;
  AccelAxes accel_axes;
  std::int8_t temperature[2];
};

#endif
