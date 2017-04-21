#include "Scenario.h"
#include <cstring>

bool Scenario::availableMaskInstruction() {
  return !m_dataFiFo.empty();
}


ncAtomicInstruction Scenario::getNextInstruction() {
  ncAtomicInstruction _instruction;
  if (m_dataFiFo.empty())
    std::memset(&_instruction, 0, sizeof(ncAtomicInstruction));
  else {
    _instruction = m_dataFiFo.front();
    m_dataFiFo.pop();
  }
  return _instruction;
}

void Scenario::clearInstructions() {
  std::queue<ncAtomicInstruction> empty;
  std::swap(this->m_dataFiFo, empty);
}

void Scenario::pushInstruction(ncAtomicInstruction &instruction) {
  this->m_dataFiFo.push(instruction);
}

void Scenario::pushInstructions(ncAtomicInstruction *instrs,
                                     std::size_t cnt) {
  for (std::size_t i = 0; i < cnt; i++) {
    this->pushInstruction(instrs[i]);
  }
}
