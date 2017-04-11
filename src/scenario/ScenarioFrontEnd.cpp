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
#include "NeuroonMaskScenariosApi.h"
#include "BaseScenario.h"
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
  bool update(const ScenarioInput *updateArgs);
  LightBoostScenario() :
    lenthInSeconds(0),
    intensity(0) {}
};

bool LightBoostScenario::update(const ScenarioInput *updateArgs){
  return false;
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

ncLucidDreamScenario * ncLucidInitScenario(ncLucidDreamScenarioArgs initArgs){
  auto *_scenario = new LucidDreamScenario;

  _scenario->remStabilityTreshold = initArgs.remStabilityTreshold;
  _scenario->startingIntensity = initArgs.startingIntensity;
  _scenario->loadDefaultScenario();

  return _scenario;
}

void ncLbDestroyScenario(ncLightBoostScenario *_scenario){
  delete _scenario;
}

ncAtomicInstruction ncLbGetNextMaskInstruction(ncLightBoostScenario *scenario){
  return getNextMaskInstruction(scenario);
}

ncMaskInstructionList ncLbGetMaskInstructions(ncLightBoostScenario *scenario,
                         const ncLightBoostScenarioInput *updateArgs){
  ncMaskInstructionList retVal = {0, NULL};
  return retVal;
}

bool ncLucidUpdate(ncLucidDreamScenario *scenario, const ncLucidDreamScenarioInput *updateArgs){
  return scenario->update(reinterpret_cast<const ScenarioInput *>(updateArgs));
}

bool ncLucidAvailableMaskInstruction(ncLucidDreamScenario *scenario){
  return scenario->availableMaskInstruction();
}

ncAtomicInstruction ncLucidGetNextMaskInstruction(ncLucidDreamScenario *scenario){
  return getNextMaskInstruction(scenario);
}

static ncAtomicInstruction getNextMaskInstruction(BaseScenario *scenario){
  return scenario->getNextInstruction();
}
