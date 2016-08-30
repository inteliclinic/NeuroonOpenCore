#include "NeuroonSignalFrames.h"
#include "Exceptions.h"
#include "logger.h"


template<typename T>
T bytes_to_int(const unsigned char* bytes, NeuroonFrameBytes::ByteOrder bo) {
  T result = 0;
  switch(bo){
  case NeuroonFrameBytes::ByteOrder::LE:{
    for (int n = sizeof( result ); n >= 0; n--)
      result = (result << 8) + bytes[ n ];
  }
    break;
  case NeuroonFrameBytes::ByteOrder::BE:{
    for (unsigned n = 0; n < sizeof( result ); n++)
      result = (result << 8) + bytes[ n ];
  }
    break;
  }
  return result;
}

template<typename T>
T bytes_to_int(const char* bytes, NeuroonFrameBytes::ByteOrder  little_endian) {
  return bytes_to_int<T>((const unsigned char*)bytes, little_endian);
}

EegFrame EegFrame::from_bytes_array(const char* bytes, std::size_t size, NeuroonFrameBytes::ByteOrder bo){
  if(size != NeuroonSignalFrame::FrameSizeBytes){
    LOG(ERROR) << "Passed size should be equal to the frame size.";
    throw std::length_error("EegFrame construction: Passed size should be equal to the frame size.");
  }
  EegFrame ef;
  ef.timestamp=bytes_to_int<std::uint32_t>(bytes, bo);
  for(std::size_t i=0;i<EegFrame::Length;i++){
    ef.signal[i]=bytes_to_int<std::int16_t>((bytes+2*i+4),bo);
  }
  return ef;
}

AccelLedsTempFrame AccelLedsTempFrame::from_bytes_array(const char* bytes, std::size_t size, NeuroonFrameBytes::ByteOrder bo){
  if(size != NeuroonSignalFrame::FrameSizeBytes){
    throw std::length_error("EegFrame construction: Passed size should be equal to the frame size.");
  }
  AccelLedsTempFrame af;
  af.timestamp=bytes_to_int<std::uint32_t>(bytes,bo);
  af.ir_led = bytes_to_int<std::int32_t>(bytes+4, bo);
  af.red_led = bytes_to_int<std::int32_t>(bytes+8, bo);

  af.accel_axes.x = bytes_to_int<std::int16_t>(bytes+12, bo);
  af.accel_axes.y = bytes_to_int<std::int16_t>(bytes+14, bo);
  af.accel_axes.z = bytes_to_int<std::int16_t>(bytes+16, bo);

  af.temperature[0] = bytes_to_int<std::int8_t>(bytes+18, bo);
  af.temperature[1] = bytes_to_int<std::int8_t>(bytes+19, bo);

  return af;
}
