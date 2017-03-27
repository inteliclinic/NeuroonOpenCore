/**
 * @file    NeuroonHAL.cpp
 * @Author  Paweł Kaźmierzewski <p.kazmierzewski@inteliclinic.com>
 * @date    February, 2017
 * @brief   Brief description
 *
 * Description
 */

#include "NeuroonHAL.h"
#include "ic_frame_handle.h"
#include <cstring>
#include <cstddef>
#include <iostream>

void ncNeuroonHalInit(ncCharacteristicsNames chars){
  nuc_init(chars);
}

int ncGetEEGCharacteristicIndex(){
  return DATA_STREAM0_UUID;
}

int ncGetPATCharacteristicIndex(){
  return DATA_STREAM1_UUID;
}

int ncGetStatusCharacteristicIndex(){
  return STATUS_STREAM_UUID;
}

int ncGetResponseCharacteristicIndex(){
  return RESPONSE_UUID;
}

int ncGetCMDCharacteristicIndex(){
  return CMD_UUID;
}

int ncGetDFUSettingsTxCharacteristicIndex(){
  return SETTINGS_TX_UUID;
}

int ncGetDFUTxCharacteristicIndex(){
  return DFU_TX_UUID;
}

int ncGetDFUSettingsRxCharacteristicIndex(){
  return SETTINGS_RX_UUID;
}

int ncGetDFURxCharacteristicIndex(){
  return DFU_RX_UUID;
}

