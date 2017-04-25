#include "Scenario.h"
#include <cstring>

bool Scenario::availableMaskInstruction() { return !_data_queue.empty(); }

ncAtomicInstruction Scenario::getNextInstruction() {
  ncAtomicInstruction _instruction;
  if (_data_queue.empty())
    std::memset(&_instruction, 0, sizeof(ncAtomicInstruction));
  else {
    _instruction = _data_queue.top();
    _data_queue.pop();
  }
  return _instruction;
}

void Scenario::clearInstructions() {
  std::priority_queue<ncAtomicInstruction, std::vector<ncAtomicInstruction>,
                      Cmp>
      empty;
  std::swap(this->_data_queue, empty);
}

ncAtomicInstruction Scenario::_adaptTimestamp(const ncAtomicInstruction &instr,
                                              InstructionInsertMode mode) {
  auto ret = instr;
  switch (mode) {
  case InstructionInsertMode::ABSOLUTE:
    break;

  case InstructionInsertMode::NOW:
    ret.time = this->currentMoment();
    break;

  case InstructionInsertMode::LAST:
    if (!this->_data_queue.empty() &&
        this->_last_instruction.time > instr.time) {
      ret.time = this->_last_instruction.time + 1;
    }
    break;
  }
  return ret;
}

bool Scenario::_insertInstructionHelper(const ncAtomicInstruction &instr) {
  bool ret = false;
  if ((!this->_data_queue.empty() &&
       this->_last_instruction.time <= instr.time) ||
      !this->_data_queue.empty()) {
    this->_last_instruction = instr;
    ret = true;
  }
  this->_data_queue.push(instr);
  return ret;
}

void Scenario::pushInstruction(const ncAtomicInstruction &instruction,
                               InstructionInsertMode mode) {
  this->_insertInstructionHelper(this->_adaptTimestamp(instruction, mode));
}

void Scenario::pushInstructions(const std::vector<ncAtomicInstruction> &instrs,
                                InstructionInsertMode mode) {
  if (instrs.empty())
    return;
  auto ains = this->_adaptTimestamp(instrs[0], mode);
  auto ts_diff = ains.time - instrs[0].time;
  for (auto ins : instrs) {
    ains = ins; // copy to leave input args unchanged
    ains.time += ts_diff;
    this->pushInstruction(ins, InstructionInsertMode::ABSOLUTE);
  }
}

void Scenario::pushInstructions(const ncAtomicInstruction *instrs,
                                std::size_t cnt, InstructionInsertMode mode) {
  return this->pushInstructions({instrs, instrs + cnt}, mode);
}
