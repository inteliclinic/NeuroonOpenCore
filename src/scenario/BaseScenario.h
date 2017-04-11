/**
 * @file    BaseScenario.h
 * @Author  Paweł Kaźmierzewski <p.kazmierzewski@inteliclinic.com>
 * @date    April, 2017
 * @brief   Brief description
 *
 * Description
 */

#ifndef BASESCENARIO_H
#define BASESCENARIO_H

#include <cstring>
#include <queue>
#include "NeuroonMaskScenariosApi.h"

union ScenarioInput{
  ncLucidDreamScenarioInput lucidDream;
  ncWakeUpScenarioInput wakeUp;
  ncPowernapScenarioInput powerNap;
  ncLightBoostScenarioInput lightBoost;
  ncCircadianRhythmAdjustmentScenarioInput circadianRyth;
};

class BaseScenario {
  public:
  virtual bool update(const ScenarioInput *) = 0;
  bool availableMaskInstruction();
  ncAtomicInstruction getNextInstruction();
  void pushInstruction(ncAtomicInstruction &instruction);
  void loadDefaultScenario();
  private:
  std::queue<ncAtomicInstruction> m_dataFiFo;
};

#endif /* !BASESCENARIO_H */
