/**
 * @file    NeuroonSunriseMicroScenario.h
 * @Author  Michał Adamczyk <m.adamczyk@inteliclinic.com>
 * @date    April, 2017
 * @brief   Micro scenario implementing Neuroon Sunrise (Artificial Dawn)
 *
 * Micro scenario implementing hard wakeup algorithm.
 * [TODO]
 *
 */

#ifndef NEUROONSUNRISEMICROSCENARIO_H
#define NEUROONSUNRISEMICROSCENARIO_H

#include "IScenarioTrigger.h"
#include "MicroScenario.h"
#include "NeuroonApiCommons.h"
#include "TimeTrigger.h"
#include <vector>

class NeuroonSunriseMicroScenario : public MicroScenario {

private:
  const int kNeuroonsunriseTime = 1;

public:

  virtual ~NeuroonSunriseMicroScenario() {}

  virtual MicroScenarioUpdate update(bool did_activate,
                                     bool last_instructions) override;

};

#endif /* !NEUROONSUNRISEMICROSCENARIO_H */
