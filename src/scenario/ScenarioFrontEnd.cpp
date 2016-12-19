/**
 * @file    ScenariosFrontEnd.cpp
 * @Author  Paweł Kaźmierzewski <p.kazmierzewski@inteliclinic.com>
 * @date    December, 2016
 * @brief   Brief description
 *
 * Description
 */

#include <queue>
#include <cstring>
#include "NeuroonMaskScenariosApi.h"
#include "ic_low_level_control.h"

struct BaseScenario{
  std::queue<ncAtomicInstruction> dataFiFo;
};

struct LucidDreamScenario:BaseScenario{
  ncLucidPulsesIntensity startingIntensity;
  ncLucidRemStabilityTreshold remStabilityTreshold;
};

struct WakeUpScenario:BaseScenario{
};

struct PowernapScenario:BaseScenario{
};

struct CircadianRhythmAdjustmentScenario:BaseScenario{
};

struct LightBoostScenario:BaseScenario{
};

ncLucidDreamScenario *ncLucidInitScenario(ncLucidDreamScenarioArgs initArgs){
  LucidDreamScenario *scenario = new LucidDreamScenario;

  scenario->remStabilityTreshold = initArgs.remStabilityTreshold;
  scenario->startingIntensity = initArgs.startingIntensity;

  return scenario;
}

ncAtomicInstruction ncLucidGetNextMaskInstruction(ncLucidDreamScenario *scenario){
  ncAtomicInstruction _instruction;
  if (scenario->dataFiFo.empty())
    std::memset(&_instruction, 0, sizeof(ncAtomicInstruction));
  else{
    _instruction = scenario->dataFiFo.front();
    scenario->dataFiFo.pop();
  }
  return _instruction;
}

static std::queue<ncAtomicInstruction> loadDefaultScenario(void){
  std::queue<ncAtomicInstruction> _queue;
  ncAtomicInstruction _instruction;
  size_t _len = sizeof(_instruction.data);

  rgb_led_set_func(_instruction.data, &_len, RGB_LED_SIDE_BOTH, FUN_TYPE_OFF, RGB_LED_COLOR_CUSTOM,
      0, 0, 0);
  _instruction.time = 100;
  _queue.push(_instruction);

  pwr_led_set_func(_instruction.data, &_len, FUN_TYPE_OFF, 0, 0, 0);
  _instruction.time = 200;
  _queue.push(_instruction);

  vibrator_set_func(_instruction.data, &_len, FUN_TYPE_OFF, 0, 0, 0);
  _instruction.time = 300;
  _queue.push(_instruction);

  return _queue;
}


