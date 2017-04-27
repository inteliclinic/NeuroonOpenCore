#ifndef __NC_SCENARIO__
#define __NC_SCENARIO__

#include "NeuroonMaskScenariosApi.h"
#include <map>
#include <queue>
#include <set>
#include <vector>

enum class InstructionInsertMode { NOW, LAST, ABSOLUTE };

class Scenario {

public:
  bool availableMaskInstruction();
  ncAtomicInstruction getNextInstruction();

  virtual ~Scenario() {}

protected:
  virtual ncUnixTimestamp currentMoment() const = 0;
  void clearInstructions();

  void
  pushInstruction(const ncAtomicInstruction &instruction,
                  InstructionInsertMode mode = InstructionInsertMode::LAST);
  void
  pushInstructions(const std::vector<ncAtomicInstruction> &instrs,
                   InstructionInsertMode mode = InstructionInsertMode::LAST);
  void
  pushInstructions(const ncAtomicInstruction *instrs, std::size_t sz,
                   InstructionInsertMode mode = InstructionInsertMode::LAST);

private:
  struct Chronological {
    bool operator()(const ncAtomicInstruction &lhs,
                    const ncAtomicInstruction &rhs) {
      return lhs.time > rhs.time;
    }
  };

  std::priority_queue<ncAtomicInstruction, std::vector<ncAtomicInstruction>,
                      Chronological>
      _data_queue;

  bool _insertInstructionHelper(const ncAtomicInstruction &instr);
  ncAtomicInstruction _adaptTimestamp(const ncAtomicInstruction &instr,
                                      InstructionInsertMode mode);
  ncAtomicInstruction _last_instruction;
};

#endif
