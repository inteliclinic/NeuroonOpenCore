#include "NeuroonSignalFrames.h"
#include "Exceptions.h"
#include "logger.h"


#define INT8_FROM_BYTES_BE(b) *(std::int8_t*)bytes
#define INT16_FROM_BYTES_BE(b) *(std::int16_t*)bytes
#define UINT32_FROM_BYTES_BE(b) *(std::uint32_t*)bytes
#define INT32_FROM_BYTES_BE(b) *(std::int32_t*)bytes


EegFrame EegFrame::from_bytes_array(char* bytes, std::size_t size, NeuroonFrameBytes::ByteOrder bo){
  if(size != NeuroonSignalFrame::FrameSizeBytes){
    LOG(ERROR) << "Passed size should be equal to the frame size.";
    throw std::length_error("EegFrame construction: Passed size should be equal to the frame size.");
  }
  EegFrame ef;
  switch(bo){
  case NeuroonFrameBytes::ByteOrder::BE:{
    ef.timestamp=UINT32_FROM_BYTES_BE(bytes);
    for(std::size_t i=0;i<EegFrame::Length;i++){
      ef.signal[i]=INT16_FROM_BYTES_BE(4+bytes+2*i);
    }
    break;
  }
  case NeuroonFrameBytes::ByteOrder::LE:{
    throw NotImplemented("TODO: Little endian conversion");
  }
  }
  return ef;
}

AccelLedsTempFrame AccelLedsTempFrame::from_bytes_array(char* bytes, std::size_t size, NeuroonFrameBytes::ByteOrder bo){
  if(size != NeuroonSignalFrame::FrameSizeBytes){
    throw std::length_error("EegFrame construction: Passed size should be equal to the frame size.");
  }
  AccelLedsTempFrame af;
  switch(bo){
  case NeuroonFrameBytes::ByteOrder::BE:{
    af.timestamp=UINT32_FROM_BYTES_BE(bytes);
    af.accel_axes = {INT16_FROM_BYTES_BE(bytes+4), INT16_FROM_BYTES_BE(bytes+6), INT16_FROM_BYTES_BE(bytes+8)};
    af.ir_led = INT32_FROM_BYTES_BE(bytes+10);
    af.red_led = INT32_FROM_BYTES_BE(bytes+14);
    af.temperature[0] = INT8_FROM_BYTES_BE(bytes+18);
    af.temperature[1] = INT8_FROM_BYTES_BE(bytes+19);
    break;
  }
  case NeuroonFrameBytes::ByteOrder::LE:{
    throw NotImplemented("TODO: Little endian conversion");
  }
  }
  return af;
}
