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
#include "ic_characteristics.h"
#include <cstring>
#include <cstddef>
#include <iostream>

void ncNeuroonHalInit(ncCharacteristicsNames chars){
  std::cout << __func__ << std::endl;
  std::size_t cnt = 0;

  for (int i = 0; i<5;++i){
    strncpy(chars[cnt], ic_neuroon_service_UUID, UUID_LENGTH + 1);
    strncpy(&chars[cnt++][4], neuroon_charecteristics[i], 4);
  }

  for(size_t i=0; i<4; ++i){
    strncpy(chars[cnt], ic_dfu_service_UUID, UUID_LENGTH + 1);
    strncpy(&chars[cnt++][4], dfu_charecteristics[i], 4);
  }
}
