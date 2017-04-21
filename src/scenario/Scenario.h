#ifndef __NC_SCENARIO__
#define __NC_SCENARIO__


#include "NeuroonMaskScenariosApi.h"
#include <map>
#include <set>
#include <queue>

class Scenario {

public:
  bool availableMaskInstruction();
  ncAtomicInstruction getNextInstruction();

  virtual ~Scenario(){}

protected:
  void clearInstructions();
  void pushInstruction(ncAtomicInstruction &instruction);
  void pushInstructions(ncAtomicInstruction *, std::size_t);

private:
  std::queue<ncAtomicInstruction> m_dataFiFo;
};


#endif
