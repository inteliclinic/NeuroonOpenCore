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
#include "SleepScenario.h"
#include "MacroScenario.h"
#include "GenericScenarioInstructionSets.h"
#include "LucidDreamScenario.h"
#include "LightBoostScenario.h"
#include "MocScenario.h"
#include "MicroScenarioSequenceLibrary.h"

ncScenario ncCreateScenario(ncScenarioType scenarioType, const ncScenarioInitArgs *args){
  return reinterpret_cast<ncScenario>(new MocScenario(args, scenarioType));
  /*
   *switch(scenarioType){
   *  case SCENARIO_LUCIDDREAM:
   *    return reinterpret_cast<ncScenario>(new LucidDreamScenario(args));
   *  case SCENARIO_SLEEP:
   *    return reinterpret_cast<ncScenario>(new SleepScenario(args));
   *  case SCENARIO_POWERNAP:
   *  case SCENARIO_CIRCADIANRHYTHM:
   *  case SCENARIO_LIGHTBOOST:
   *    return NULL;
   *}
   */
}

ncAtomicInstruction ncGetNextInstruction(ncScenario scenario){
  return reinterpret_cast<MacroScenario *>(scenario)->getNextInstruction();
}

ncUpdateOutput ncScenarioUpdate(ncScenario scenario, const ncScenarioInput *updateArgs){
  return reinterpret_cast<MacroScenario *>(scenario)->update(updateArgs);
}

ncAtomicInstruction ncDestroyScenario(ncScenario scenario){
  delete reinterpret_cast<MacroScenario *>(scenario);
  return {};
}

bool ncAvailableMaskInstruction(ncScenario scenario){
  return reinterpret_cast<MacroScenario *>(scenario)->availableMaskInstruction();
}
