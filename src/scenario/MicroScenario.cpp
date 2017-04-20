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

bool MicroScenario::availableMaskInstruction() {
  return !m_dataFiFo.empty();
}

ncAtomicInstruction MicroScenario::getNextInstruction() {
  ncAtomicInstruction _instruction;
  if (m_dataFiFo.empty())
    std::memset(&_instruction, 0, sizeof(ncAtomicInstruction));
  else {
    _instruction = m_dataFiFo.front();
    m_dataFiFo.pop();
  }
  return _instruction;
}

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

void MicroScenario::clearInstructions() {
  std::queue<ncAtomicInstruction> empty;
  std::swap(this->m_dataFiFo, empty);
}

void MicroScenario::pushInstruction(ncAtomicInstruction &instruction) {
  this->m_dataFiFo.push(instruction);
}

void MicroScenario::pushInstructions(ncAtomicInstruction *instrs,
                                         std::size_t cnt) {
  for (std::size_t i = 0; i < cnt; i++) {
    this->pushInstruction(instrs[i]);
  }
}



