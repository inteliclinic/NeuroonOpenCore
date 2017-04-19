/**
 * @file    MacroScenario.cpp
 * @Author  Paweł Kaźmierzewski <p.kazmierzewski@inteliclinic.com>
 * @date    April, 2017
 * @brief   Brief description
 *
 * Description
 */

#include <cstring>
#include "GenericScenarioTemplates.h"
#include "MacroScenario.h"

MacroScenario::MacroScenario(){
}

bool MacroScenario::availableMaskInstruction(){
  return !m_dataFiFo.empty();
}

void MacroScenario::pushInstruction(ncAtomicInstruction &instruction){
  m_dataFiFo.push(instruction);
}

ncAtomicInstruction MacroScenario::getNextInstruction(){
  ncAtomicInstruction _instruction;
  if (m_dataFiFo.empty())
    std::memset(&_instruction, 0, sizeof(ncAtomicInstruction));
  else{
    _instruction = m_dataFiFo.front();
    m_dataFiFo.pop();
  }
  return _instruction;
}

void MacroScenario::loadDefaultScenario(){
  killAllDevices(m_dataFiFo);
}

MacroScenario::~MacroScenario(){}
