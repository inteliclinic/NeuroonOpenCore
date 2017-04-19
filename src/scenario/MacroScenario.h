/**
 * @file    MacroScenario.h
 * @Author  Paweł Kaźmierzewski <p.kazmierzewski@inteliclinic.com>
 * @date    April, 2017
 * @brief   Brief description
 *
 * Description
 */

#ifndef MACROSCENARIO_H
#define MACROSCENARIO_H

#include <map>
#include "MicroScenario.h"
#include "TriggerScenario.h"

class MacroScenario{
  public:
    MacroScenario();
    virtual ncUpdateOutput update(const ncScenarioInput *) = 0;
    virtual ~MacroScenario() = 0;
    bool availableMaskInstruction();
    ncAtomicInstruction getNextInstruction();
    void pushInstruction(ncAtomicInstruction &instruction);

  protected:
    void loadDefaultScenario();
    std::map<TriggerScenario *, MicroScenario *> m_triggerMap;

  private:
    std::queue<ncAtomicInstruction> m_dataFiFo;
};

#endif /* !MACROSCENARIO_H */
