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
  bool m_sequenceGenerated;
  bool m_trigerAcknowledged;
  enum TriggersIDs{
    ACTIVATIONTRIGGER, SIGNALLOST, REMPHASE
  };

public:
  LucidDreamLightsScenario();
  ~LucidDreamLightsScenario() {}

  MicroScenarioUpdate update(bool did_activate, bool last_instructions) override;

  void installRemPhaseTrigger(std::shared_ptr<const IScenarioTrigger> t) {
    this->installTriggerForKey(t, TriggersIDs::REMPHASE);
  }

  /*
   *void installSignalLostTrigger(std::shared_ptr<const IScenarioTrigger> t) {
   *  this->installTriggerForKey(t, kSignalLost);
   *}
  */

  std::vector<ncAtomicInstruction> LucidSequence();
  std::vector<ncAtomicInstruction> _desequence();
};

#endif /* !LUCIDDREAMLIGHTSSCENARIO_H */
