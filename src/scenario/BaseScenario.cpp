/**
 * @file    BaseScenario.cpp
 * @Author  Paweł Kaźmierzewski <p.kazmierzewski@inteliclinic.com>
 * @date    April, 2017
 * @brief   Brief description
 *
 * Description
 */

#include "BaseScenario.h"
#include "GenericScenarioTemplates.h"

bool BaseScenario::availableMaskInstruction(){
  return !m_dataFiFo.empty();
}

void BaseScenario::pushInstruction(ncAtomicInstruction &instruction){
  m_dataFiFo.push(instruction);
}

ncAtomicInstruction BaseScenario::getNextInstruction(){
  ncAtomicInstruction _instruction;
  if (m_dataFiFo.empty())
    std::memset(&_instruction, 0, sizeof(ncAtomicInstruction));
  else{
    _instruction = m_dataFiFo.front();
    m_dataFiFo.pop();
  }
  return _instruction;
}

void BaseScenario::loadDefaultScenario(){
  killAllDevices(m_dataFiFo);
}

BaseScenario::~BaseScenario(){}
