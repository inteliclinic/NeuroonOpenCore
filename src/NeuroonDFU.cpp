/**
 * @file    NeuroonDfu.cpp
 * @Author  Paweł Kaźmierzewski <p.kazmierzewski@inteliclinic.com>
 * @date    February, 2017
 * @brief   Brief description
 *
 * Description
 */

#include "NeuroonDfuApi.h"
#include "../external_modules/unified_communication/API/ic_dfu.h"
#include <iostream>
#include <cstdint>

#define PRINT_DBG_INFO std::cout<<"running: "<<__FILE__<<"::"<<__func__<<std::endl

int ncGotoDfu(char *frame, size_t *len, ncFirmwareMilestone firmware){
  e_firmwareMilestone _firmware = firmware == NEW_NEUROON_FW ? NEW_NEUROON_FIRMWARE :
    LEGACY_NEUROON_FIRMWARE;

  return goto_dfu(frame, len, _firmware);
}

int ncDfuResponseSink(char* frame, size_t *len, char *responseFrame, size_t responseLen,
    ncDfuAction *action){
  e_dfuAction _action;
  int retVal = dfu_response_sink(responseFrame, responseLen, frame, len, &_action);
  switch(_action){
    case DFU_SEND_NEXT_DATASET:
      *action = DFU_SEND_NEXT;
    case DFU_RESEND_DATASET:
      *action = DFU_RESEND;
    case DFU_TERMINATE:
      *action = DFU_TERM;
    case DFU_END:
      *action = DFU_FINISHED;
  }
  return retVal;
}

int ncDfuStartUpdate(char *frame, size_t *len, char *fb, size_t file_len, ncFirmwareType firm,
    uint32_t version){
  e_firmwareType _firm;
  switch(firm){
    case SD_FIRM:
      _firm = SD_FIRMWARE;
      break;
    case APP_FIRM:
      _firm = APP_FIRMWARE;
      break;
    case DFU_FIRM:
      _firm = DFU_FIRMWARE;
      break;
  }
  return dfu_start_update(frame, len, fb, file_len, _firm, version);
}
