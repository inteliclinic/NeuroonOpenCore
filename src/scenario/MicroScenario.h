/**
 * @file    MicroScenario.h
 * @Author  Paweł Kaźmierzewski <p.kazmierzewski@inteliclinic.com>
 * @date    April, 2017
 * @brief   Brief description
 *
 * Description
 */

#ifndef MICROSCENARIO_H
#define MICROSCENARIO_H

#include <queue>
#include "NeuroonMaskScenariosApi.h"

class MicroScenario{
  public:
    MicroScenario();
    std::queue<ncAtomicInstruction> getScenarioSequence();
  private:
    std::queue<ncAtomicInstruction> m_microSequnceFiFo;
};

#endif /* !MICROSCENARIO_H */
