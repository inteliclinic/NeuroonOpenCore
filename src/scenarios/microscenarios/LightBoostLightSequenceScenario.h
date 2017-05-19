/**
 * @file    LightBoostLightSequenceScenario .h
 * @Author  Michał Adamczyk <m.adamczyk@inteliclinic.com>
 * @date    April, 2017
 * @brief   Micro scenario implementing light boost light sequence.
 *
 * Micro scenario implementing light boost light sequence.
 * [TODO]
 */

#ifndef LIGHTBOOSTLIGHTSEQUENCESCENARIO_H
#define LIGHTBOOSTLIGHTSEQUENCESCENARIO_H

#include "IScenarioTrigger.h"
#include "MicroScenario.h"
#include "NeuroonApiCommons.h"
#include <vector>

class LightBoostLightSequenceScenario : public MicroScenario {
private:
  const int kActivationTrigger = 1;
  std::vector<ncAtomicInstruction> _descendSequence() const;
  std::vector<ncAtomicInstruction> _parametrizedSinusLikeSequence() const;

  unsigned int m_ascendTimeMs;
  unsigned int m_crestTimeMs;
  unsigned int m_descendTimeMs;
  unsigned int m_troughTimeMs;
  unsigned int m_durationMs;


public:
  LightBoostLightSequenceScenario(unsigned int ascend_time_ms,
                                  unsigned int crest_time_ms,
                                  unsigned int descend_time_ms,
                                  unsigned int trough_time_ms,
                                  unsigned int duration_ms);

  void installActivationTrigger(std::shared_ptr<const IScenarioTrigger> t) {
    return this->installTriggerForKey(t, kActivationTrigger);
  }

protected:
  MicroScenarioUpdate update(bool did_activate,
                             bool last_instructions) override;
};

#endif /* !LIGHTBOOSTLIGHTSEQUENCESCENARIO_H */
