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
#include <vector>

class HardWakeupMicroScenario : public MicroScenario {

private:
  const int kHardwakeupTime = 1;
  const int kWokenUp = 2;
  const int kLostSignal = 3;

public:

  virtual ~HardWakeupMicroScenario() {}

  virtual MicroScenarioUpdate update(bool did_activate,
                                     bool last_instructions) override;

};

#endif /* !HARDWAKEUPMICROSCENARIO_H */
