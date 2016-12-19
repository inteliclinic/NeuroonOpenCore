/**
 * @file    GenericScenarioTemplates.cpp
 * @Author  Paweł Kaźmierzewski <p.kazmierzewski@inteliclinic.com>
 * @date    December, 2016
 * @brief   Brief description
 *
 * Description
 */

#include <cstring>
#include "ic_low_level_control.h"
#include "GenericScenarioTemplates.h"

void killAllDevices(std::queue<ncAtomicInstruction> &inQueue){
  ncAtomicInstruction _instruction;
  size_t _len = sizeof(_instruction.data);

  rgb_led_set_func(_instruction.data, &_len, RGB_LED_SIDE_BOTH, FUN_TYPE_OFF, RGB_LED_COLOR_CUSTOM,
      0, 0, 0);
  _instruction.time = 100;
  inQueue.push(_instruction);

  pwr_led_set_func(_instruction.data, &_len, FUN_TYPE_OFF, 0, 0, 0);
  _instruction.time = 200;
  inQueue.push(_instruction);

  vibrator_set_func(_instruction.data, &_len, FUN_TYPE_OFF, 0, 0, 0);
  _instruction.time = 300;
  inQueue.push(_instruction);
}
