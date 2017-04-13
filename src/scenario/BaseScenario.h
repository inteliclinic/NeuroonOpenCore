/**
 * @file    BaseScenario.h
 * @Author  Paweł Kaźmierzewski <p.kazmierzewski@inteliclinic.com>
 * @date    April, 2017
 * @brief   Brief description
 *
 * Description
 */

#ifndef BASESCENARIO_H
#define BASESCENARIO_H

#include <cstring>
#include <queue>
#include "NeuroonMaskScenariosApiNew.h"

class BaseScenario {
  public:
    virtual ncUpdateOutput update(const ncScenarioInput *) = 0;
    virtual ~BaseScenario() = 0;
    bool availableMaskInstruction();
    ncAtomicInstruction getNextInstruction();
    void pushInstruction(ncAtomicInstruction &instruction);
    void loadDefaultScenario();

  private:
    std::queue<ncAtomicInstruction> m_dataFiFo;
};

#endif /* !BASESCENARIO_H */
