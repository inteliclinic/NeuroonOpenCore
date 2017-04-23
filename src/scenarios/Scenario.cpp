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

void Scenario::pushInstruction(ncAtomicInstruction &instruction,
                               QueueInsertMode mode) {

  // [TODO]
  // switch (mode) {
  // case QueueInsertMode::Q_INSERT_NOW:
  //   instruction.time = this->currentMoment();
  //   break;
  // case QueueInsertMode::Q_INSERT_ABSOLUTE:{

  //   if(instruction.time >= this->_last_instruction->time){
  //     // [TODO]
  //     this->_last_instruction = &instruction;
  //   }
  //   break;
  // }
  // case QueueInsertMode::Q_INSERT_LAST:
  //   if(this->_data_queue.empty()){
  //     this->_data_queue.push(instruction);
  //     this->_last_instruction = &(this->_data_queue.top());
      
  //   }
  //   break;
  // }

  // if()

  // this->_data_queue.push(instruction);
}

void Scenario::pushInstructions(
                                const std::vector<ncAtomicInstruction> &instrs,
                                QueueInsertMode mode) {
  // TODO
  for (auto ins : instrs) {
    this->pushInstruction(ins);
  }
}

void Scenario::pushInstructions(ncAtomicInstruction *instrs, std::size_t cnt,
                                QueueInsertMode mode) {
  // TODO
  for (std::size_t i = 0; i < cnt; i++) {
    this->pushInstruction(instrs[i]);
  }
}
