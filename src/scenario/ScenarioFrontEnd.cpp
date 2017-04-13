/**
 * @file    ScenariosFrontEnd.cpp
 * @Author  Paweł Kaźmierzewski <p.kazmierzewski@inteliclinic.com>
 * @date    December, 2016
 * @brief   Brief description
 *
 * Description
 */

#include <queue>
#include <iostream>
#include <cstdint>
//#include "NeuroonMaskScenariosApi.h"
#include "NeuroonMaskScenariosApiNew.h"
#include "BaseScenario.h"
#include "SleepScenario.h"
#include "GenericScenarioTemplates.h"
#include "LucidDreamScenario.h"

/////////////////////////////////Michaltmp//////////////////////////////////////////////////////////
typedef struct{
  unsigned long timestamp;
  bool rem;
}ncMichalScenarioInput;
///////////////////////////////////////////////////////////////////////////////////////////////////


struct LightBoostScenario:BaseScenario{
  unsigned int lenthInSeconds;
  unsigned char intensity;
  ncUpdateOutput update(const ncScenarioInput *updateArgs);
  LightBoostScenario() :
    lenthInSeconds(0),
    intensity(0) {}
};

ncUpdateOutput LightBoostScenario::update(const ncScenarioInput *updateArgs){
  return UPDATE_OK;
}

struct WakeUpScenario:BaseScenario{
};

struct PowernapScenario:BaseScenario{
};

struct CircadianRhythmAdjustmentScenario:BaseScenario{
};

static std::queue<ncAtomicInstruction> loadDefaultScenario(void);
static ncAtomicInstruction getNextMaskInstruction(BaseScenario *scenario);
static bool availableMaskInstruction(ncLucidDreamScenario *scenario);

ncScenario ncCreateScenario(ncScenarioType scenarioType, const ncScenarioInitArgs *args){
  switch(scenarioType){
    case SCENARIO_LUCIDDREAM:
      return reinterpret_cast<ncScenario>(new LucidDreamScenario(args));
    case SCENARIO_SLEEP:
      return reinterpret_cast<ncScenario>(new SleepScenario(args));
    case SCENARIO_POWERNAP:
    case SCENARIO_CIRCADIANRHYTHM:
    case SCENARIO_LIGHTBOOST:
      return NULL;
  }
}

ncAtomicInstruction ncGetNextInstruction(ncScenario scenario){
  return reinterpret_cast<BaseScenario *>(scenario)->getNextInstruction();
}

ncUpdateOutput ncScenarioUpdate(ncScenario scenario, const ncScenarioInput *updateArgs){
  return reinterpret_cast<BaseScenario *>(scenario)->update(updateArgs);
}

void ncDestroyScenario(ncScenario scenario){
  delete reinterpret_cast<BaseScenario *>(scenario);
}
bool ncAvailableMaskInstruction(ncScenario scenario){
  return reinterpret_cast<BaseScenario *>(scenario)->availableMaskInstruction();
}
