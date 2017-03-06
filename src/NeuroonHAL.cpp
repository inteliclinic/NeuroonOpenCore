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
