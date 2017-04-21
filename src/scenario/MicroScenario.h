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
#include "Scenario.h"
#include <map>
#include <queue>
#include <set>

typedef int Key;

class MicroScenario : public Scenario {
public:
  ncUpdateOutput refresh();
  void uninstallTrigger(IScenarioTrigger *);

protected:
  void installTriggerForKey(const IScenarioTrigger *, Key);
  virtual ncUpdateOutput update(const std::set<Key> &) = 0;

private:
  std::map<const IScenarioTrigger *, Key> _triggers;
};

#endif /* !MICROSCENARIO_H */
