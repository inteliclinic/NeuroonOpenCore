#ifndef __NC_SCENARIO__
#define __NC_SCENARIO__

#include "NeuroonMaskScenariosApi.h"
#include <map>
#include <queue>
#include <set>
#include <vector>

enum class QueueInsertMode{ Q_INSERT_NOW, Q_INSERT_LAST, Q_INSERT_ABSOLUTE };

class Scenario {

public:
  bool availableMaskInstruction();
  ncAtomicInstruction getNextInstruction();

  virtual ~Scenario() {}

protected:

  virtual ncUnixTimestamp currentMoment() const = 0;
  void clearInstructions();

  void pushInstruction(ncAtomicInstruction &instruction, QueueInsertMode mode=QueueInsertMode::Q_INSERT_LAST);
  void pushInstructions(const std::vector<ncAtomicInstruction> & instrs,QueueInsertMode mode=QueueInsertMode::Q_INSERT_LAST);
  void pushInstructions(ncAtomicInstruction * instrs, std::size_t sz,QueueInsertMode mode=QueueInsertMode::Q_INSERT_LAST);

private:
  struct Cmp
  {
    bool operator()(const ncAtomicInstruction& lhs, const ncAtomicInstruction& rhs)
    {
      return lhs.time>rhs.time;
    }
  };

  std::priority_queue<ncAtomicInstruction, std::vector<ncAtomicInstruction>, Cmp> _data_queue;
  ncAtomicInstruction* _last_instruction = nullptr;
};

#endif
