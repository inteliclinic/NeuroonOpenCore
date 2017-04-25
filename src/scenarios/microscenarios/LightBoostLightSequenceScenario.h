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
  bool _is_active = false;

public:
  LightBoostLightSequenceScenario(unsigned int ascend_time_ms,
                                  unsigned int crest_time_ms,
                                  unsigned int descend_time_ms,
                                  unsigned int trough_time_ms) {}

  void installActivationTrigger(IScenarioTrigger *t) {
    return this->installTriggerForKey(t, kActivationTrigger);
  }

  bool isActive() const { return _is_active; }

protected:
  ncUpdateOutput go(const std::set<Key> &triggers) override {
    return UPDATE_OK;
  }

  ncUpdateOutput onFinish() override { return UPDATE_OK; }
  ncUpdateOutput onMute() override { return UPDATE_OK; }
};

#endif /* !LIGHTBOOSTLIGHTSEQUENCESCENARIO_H */
