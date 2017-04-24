/**
 * @file    ContactTestScenario.h
 * @Author  Michał Adamczyk <m.adamczyk@inteliclinic.com>
 * @date    April, 2017
 * @brief   Brief description
 *
 * Micro scenario used for warning user about bad signal
 * or lost electrode contact
 */

#ifndef CONTACTTESTSCENARIO_H
#define CONTACTTESTSCENARIO_H

#include "IScenarioTrigger.h"
#include "MicroScenario.h"
#include "NeuroonApiCommons.h"
#include <vector>

class ContactTestScenario : public MicroScenario {

private:
  const int kSignalLost = 1;
  const int kDontWakeTimer = 2;

  ncUnixTimestamp _lights_would_turn_on_at = 0;
  ncUnixTimestamp _lights_would_turn_off_at = 0;

  int _r, _g, _b;
  float _intensity;

  std::vector<ncAtomicInstruction> _lights_on_instructions = {};
  std::vector<ncAtomicInstruction> _lights_off_instructions{};

  const std::vector<ncAtomicInstruction> &_getLightsOnInstructions();
  const std::vector<ncAtomicInstruction> &_getLightsOffInstructions();

  ncUpdateOutput _turnOffOrCancelLights();
  ncUpdateOutput _turnOnOrRescheduleLights();

public:
  ContactTestScenario() : ContactTestScenario(0xff, 0x80, 0x00, 0.9) {}

  ContactTestScenario(int r, int g, int b, float intensity = 1.0)
      : _r(r), _g(g), _b(b), _intensity(intensity) {}

  ncUpdateOutput go(const std::set<Key> &triggers) override;
  ncUpdateOutput onFinish() override;
  ncUpdateOutput onMute() override;

  void installSignalLostTrigger(const IScenarioTrigger *t) {
    this->installTriggerForKey(t, kSignalLost);
  }

  void installDontWakeTrigger(const IScenarioTrigger *t) {
    this->installTriggerForKey(t, kDontWakeTimer);
  }
};

#endif /* !CONTACTTESTSCENARIO_H */
