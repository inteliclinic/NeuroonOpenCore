/**
 * @file    HardwakeupMicroScenario.h
 * @Author  Michał Adamczyk <m.adamczyk@inteliclinic.com>
 * @date    April, 2017
 * @brief   Micro scenario implementing hard wakeup algorithm.
 *
 * Micro scenario implementing hard wakeup algorithm.
 * [TODO]
 *
 */

#ifndef HARDWAKEUPMICROSCENARIO_H
#define HARDWAKEUPMICROSCENARIO_H

#include "IScenarioTrigger.h"
#include "MicroScenario.h"
#include "NeuroonApiCommons.h"
#include "TimeTrigger.h"
#include "NeuroonMaskScenariosApi.h"
#include <vector>

typedef enum{
  WAKEUP_ALARM_SOFT = 0x01,
  WAKEUP_ALARM_MEDIUM,
  WAKEUP_ALARM_HARD,
  WAKEUP_ALARM_OFF
}ncWakeUpSequenceIntensity;

class HardWakeupMicroScenario : public MicroScenario {

private:
  const int kHardwakeupTime = 1;
  const int kWokenUp = 2;
  const int kLostSignal = 3;

  bool m_activationRegistered;
  ncWakeUpSequenceIntensity m_intensity;

public:

  HardWakeupMicroScenario(ncWakeUpSequenceIntensity);
  virtual ~HardWakeupMicroScenario() {}

  virtual MicroScenarioUpdate update(bool did_activate,
                                     bool last_instructions) override;

  void installActivationTrigger(std::shared_ptr<const IScenarioTrigger> t) {
    return this->installTriggerForKey(t, kHardwakeupTime);
  }

};

#endif /* !HARDWAKEUPMICROSCENARIO_H */
