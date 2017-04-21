/**
 * @file    MicroScenario.cpp
 * @Author  Michał Adamczyk <m.adamczyk@inteliclinic.com>
 * @Author  Paweł Kaźmierzewski <p.kazmierzewski@inteliclinic.com>
 * @date    April, 2017
 * @brief   Brief description
 *
 * Description
 */

#include "MicroScenario.h"
#include <cstring>
#include <algorithm>


ncUpdateOutput MicroScenario::refresh() {
  // run specific scenario update only on active triggers
  std::set<int> active_triggers;
  for(const auto & kp : this->_triggers){
    if(kp.first->isActive()){ active_triggers.insert(kp.second);}
  }
  return this->update(active_triggers);
}

void MicroScenario::installTriggerForKey(const IScenarioTrigger *t, Key k) {
  this->_triggers[t] = k;
}

void MicroScenario::uninstallTrigger(IScenarioTrigger *t) {
  this->_triggers.erase(t);
}
