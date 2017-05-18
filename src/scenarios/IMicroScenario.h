/**
 * @file    IMicroScenario.h
 * @Author  Michał Adamczyk <m.adamczyk@inteliclinic.com>
 * @date    April, 2017
 * @brief   Provides an abstract interface for using trigger-based
 * micro-scenarios.
 *
 * Description [TODO]
 */

#ifndef IMICROSCENARIO_H
#define IMICROSCENARIO_H

#include "NeuroonMaskScenariosApi.h"
#include <vector>

enum class MicroScenarioState { WANTS_FOCUS, IDLE, FINISHED };

class IMicroScenario {
public:
  virtual ~IMicroScenario() {}

  virtual MicroScenarioState getState() const = 0;

  virtual void refresh(ncUnixTimestamp) = 0;
  virtual void finish() = 0;

  virtual std::vector<ncAtomicInstruction> use() = 0;
  virtual std::vector<ncAtomicInstruction> stopUse() = 0;
};
#endif
