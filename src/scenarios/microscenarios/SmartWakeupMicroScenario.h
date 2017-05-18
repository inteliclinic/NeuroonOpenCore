/**
 * @file    SmartwakeupMicroScenario.h
 * @Author  Michał Adamczyk <m.adamczyk@inteliclinic.com>
 * @date    April, 2017
 * @brief   Micro scenario implementing smart wakeup algorithm.
 *
 * Micro scenario implementing smart wakeup algorithm.
 * [TODO]
 *
 */

#ifndef SMARTWAKEUPMICROSCENARIO_H
#define SMARTWAKEUPMICROSCENARIO_H

#include "IScenarioTrigger.h"
#include "MicroScenario.h"
#include "NeuroonApiCommons.h"
#include "TimeTrigger.h"
#include <vector>

class SmartWakeupMicroScenario : public MicroScenario {

private:
  const int kSmartwakeupWindow = 1;
  const int kRemPhase= 2;
  const int kLightPhase= 3;
  const int kAlmostWokenUp= 4;

public:

  virtual ~SmartWakeupMicroScenario() {}

  virtual MicroScenarioUpdate update(bool did_activate,
                                     bool last_instructions) override;

};

#endif /* !SMARTWAKEUPMICROSCENARIO_H */
