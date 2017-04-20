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

#include "IScenarioTrigger.h"
#include "NeuroonMaskScenariosApi.h"
#include <map>
#include <set>
#include <queue>

typedef int Key;

class MicroScenario {
public:
  ncUpdateOutput refresh();

  bool availableMaskInstruction();
  ncAtomicInstruction getNextInstruction();

  void uninstallTrigger(IScenarioTrigger *);

protected:
  void installTriggerForKey(const IScenarioTrigger *, Key);
  virtual ncUpdateOutput update(const std::set<Key>&) = 0;
  void clearInstructions();
  void pushInstruction(ncAtomicInstruction &instruction);
  void pushInstructions(ncAtomicInstruction *, std::size_t);

private:
  std::queue<ncAtomicInstruction> m_dataFiFo;
  std::map<const IScenarioTrigger *, Key> _triggers;
};

#endif /* !MICROSCENARIO_H */
