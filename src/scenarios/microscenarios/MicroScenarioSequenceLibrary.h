/**
 * @file    MicroScenarioSequenceLibrary.h
 * @Author  Wojciech Weclewski <w.weclewski@inteliclinic.com>
 * @date    May, 2017
 * @brief   Brief description
 *
 * Description
 */

#ifndef MICROSCENARIOSEQUENCELIBRARY_H
#define MICROSCENARIOSEQUENCELIBRARY_H

#include <vector>
#include "NeuroonMaskScenariosApi.h"

typedef enum{
  WU_ALARM_SOFT = 0x01,
  WU_ALARM_MEDIUM,
  WU_ALARM_HARD,
  WU_ALARM_OFF
}wakeUpSequenceIntensity;

namespace sequence{

  std::vector<ncAtomicInstruction> descendSequence(unsigned int, ncUnixTimestamp);
  std::vector<ncAtomicInstruction> parametrizedSinusLikeSequence(unsigned int, unsigned int,
      unsigned int, unsigned int, unsigned int, ncUnixTimestamp);
  std::vector<ncAtomicInstruction> artificialDawnSequence(unsigned int, ncUnixTimestamp);
  std::vector<ncAtomicInstruction> wakeUpSequence(wakeUpSequenceIntensity, ncUnixTimestamp);
  std::vector<ncAtomicInstruction> lucidDreamSequence(ncUnixTimestamp);
  std::vector<ncAtomicInstruction> circadianShiftSequence(ncUnixTimestamp);
  std::vector<ncAtomicInstruction> noContactSequence(ncUnixTimestamp);
  std::vector<ncAtomicInstruction> killSequence(ncUnixTimestamp);
}

namespace msequence{
  std::vector<ncAtomicInstruction> lucidDreamMocSequence(ncUnixTimestamp);
  std::vector<ncAtomicInstruction> lightBoostMocSequence(ncUnixTimestamp);
  std::vector<ncAtomicInstruction> sleepMocSequence(ncUnixTimestamp);
  std::vector<ncAtomicInstruction> powerNapMocSequence(ncUnixTimestamp);
  std::vector<ncAtomicInstruction> circadianRythmMocSequence(ncUnixTimestamp);
}

#endif /* !MICROSCENARIOSEQUENCELIBRARY_H */
