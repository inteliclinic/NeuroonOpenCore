/**
 * @file    LucidDreamLightsScenario.h
 * @Author  Michał Adamczyk <m.adamczyk@inteliclinic.com>
 * @date    April, 2017
 * @brief   Micro scenario used for realisation of lucid dream function.
 *
 * Micro scenario used for realisation of lucid dream scenario,
 * It is responsible for generation of light patterns that
 * will be acivated during user's REM phase.
 *
 */

#ifndef LUCIDDREAMLIGHTSSCENARIO_H
#define LUCIDDREAMLIGHTSSCENARIO_H

#include "IScenarioTrigger.h"
#include "MicroScenario.h"
#include "NeuroonApiCommons.h"
#include "TimeTrigger.h"
#include <vector>

class LucidDreamLightsScenario : public MicroScenario {

private:
  const int kSignalLost = 1;
  const int kRemPhase = 2;

public:
  virtual ~LucidDreamLightsScenario() {}

  virtual MicroScenarioUpdate update(bool did_activate,
                                     bool last_instructions) override;

  void installRemPhaseTrigger(std::shared_ptr<const IScenarioTrigger> t) {
    this->installTriggerForKey(t, kRemPhase);
  }

  void installSignalLostTrigger(std::shared_ptr<const IScenarioTrigger> t) {
    this->installTriggerForKey(t, kSignalLost);
  }
};

#endif /* !LUCIDDREAMLIGHTSSCENARIO_H */
