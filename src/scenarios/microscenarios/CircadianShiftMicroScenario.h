/**
 * @file    CircadianShiftMicroScenario.h
 * @Author  Michał Adamczyk <m.adamczyk@inteliclinic.com>
 * @date    April, 2017
 * @brief   Micro scenario implementing circadian shift algorithm
 *
 * [TODO]
 *
 */

#ifndef CIRCADIANSHIFTMICROSCENARIO_H
#define CIRCADIANSHIFTMICROSCENARIO_H

#include "IScenarioTrigger.h"
#include "MicroScenario.h"
#include "NeuroonApiCommons.h"
#include "TimeTrigger.h"
#include <vector>

class CircadianShiftMicroScenario : public MicroScenario {

private:
  const int kCircadianShiftTime = 1;
  const int kUnstableSleep = 2;

public:

  virtual ~CircadianShiftMicroScenario() {}
  virtual MicroScenarioUpdate update(bool did_activate,
                                     bool last_instructions) override;

};

#endif /* !CIRCADIANSHIFTMICROSCENARIO_H */
