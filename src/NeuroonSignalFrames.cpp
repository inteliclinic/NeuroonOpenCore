#include "NeuroonSignalFrames.h"
#include <string.h>

int is_big_endian(void)
{
  union {
    uint32_t i;
    char c[4];
  } bint = {0x01020304};
  return bint.c[0] == 1;
}

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

template<typename T>
void int_to_bytes(char* out, T num, NeuroonFrameBytes::ByteOrder bo) {
  auto sz = sizeof(T);
  memcpy(out, &num, sz);
  if((bo == NeuroonFrameBytes::ByteOrder::LE && is_big_endian())
     || (bo == NeuroonFrameBytes::ByteOrder::BE && !is_big_endian())){
    for(std::size_t i=0;i<sz/2;i++){
      std::swap(out[i],out[sz-i-1]);
    }
  }
}

EegFrame EegFrame::from_bytes_array(const char* bytes, std::size_t size, NeuroonFrameBytes::ByteOrder bo){
  if(size != NeuroonSignalFrame::FrameSizeBytes){
    throw std::length_error("EegFrame construction: Passed size should be equal to the frame size.");
  }
  EegFrame ef;
  ef.timestamp=bytes_to_int<std::uint32_t>(bytes, bo);
  for(std::size_t i=0;i<EegFrame::Length;i++){
    ef.signal[i]=bytes_to_int<std::int16_t>((bytes+2*i+4),bo);
  }
  return ef;
}

PatFrame PatFrame::from_bytes_array(const char* bytes, std::size_t size, NeuroonFrameBytes::ByteOrder bo){
  if(size != NeuroonSignalFrame::FrameSizeBytes){
    throw std::length_error("EegFrame construction: Passed size should be equal to the frame size.");
  }
  PatFrame af;
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


void EegFrame::to_bytes(char* out, NeuroonFrameBytes::ByteOrder bo) const{
  int_to_bytes<std::uint32_t>(out, this->timestamp, bo);
  for(std::size_t i = 0; i < EegFrame::Length; i++){
    int_to_bytes<std::int16_t>(out+4+2*i, this->signal[i], bo);
  }
}

void PatFrame::to_bytes(char* out, NeuroonFrameBytes::ByteOrder bo) const{
  int_to_bytes<std::uint32_t>(out, this->timestamp, bo);
  int_to_bytes<std::int32_t>(out+4, this->ir_led, bo);
  int_to_bytes<std::int32_t>(out+8, this->red_led, bo);
  int_to_bytes<std::int16_t>(out+12, this->accel_axes.x, bo);
  int_to_bytes<std::int16_t>(out+14, this->accel_axes.y, bo);
  int_to_bytes<std::int16_t>(out+16, this->accel_axes.z, bo);
  int_to_bytes<std::int8_t>(out+18, this->temperature[0], bo);
  int_to_bytes<std::int8_t>(out+19, this->temperature[1], bo);
}
