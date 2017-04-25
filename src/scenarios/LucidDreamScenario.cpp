/**
 * @file    LucidDreamScenario.cpp
 * @Author  Paweł Kaźmierzewski <p.kazmierzewski@inteliclinic.com>
 * @date    April, 2017
 * @brief   Brief description
 *
 * Description
 */

#include "LucidDreamScenario.h"
#include "ic_low_level_control.h"
#include <cstring>

LucidDreamScenario::LucidDreamScenario(const ncScenarioInitArgs *args)
    : SleepScenario(args),
      m_startingIntensity(args->lucidDream.startingIntensity),
      m_remStabilityTreshold(args->lucidDream.remStabilityTreshold),
      m_remCounter(0), m_remStartTimestamp(0), m_remDetected(false),
      m_remCounted(false), m_lucidLoaded(false) {}

ncUpdateOutput LucidDreamScenario::update(ncUnixTimestamp ts, const ncScenarioInput *updateArgs) {
  unsigned long long remDuration =
      updateArgs->lucidDream.timestamp - m_remStartTimestamp;

  if (m_remDetected)
    if (remDuration >= 180000 &&
        !m_remCounted) { // TODO: 180000 needs to be a parameter
      m_remCounter++;
      m_remCounted = true;
    }

  if (updateArgs->lucidDream.currentSleep_stage == REM) {
    if (!m_remDetected) {
      m_remStartTimestamp = updateArgs->lucidDream.timestamp;
      remDuration = 0;
      m_remDetected = true;
      m_remCounted = false;
    } else {
    }
  } else {
    if (m_remDetected) {
      m_remDetected = false;
      m_remCounted = false;
    }
    m_lucidLoaded = false;
  }

  switch (m_remCounter) {
  case 0:
  case 1:
  case 2:
    break;
  case 3:
    if (remDuration >= 300000) { // TODO: 300000 needs to ba a parameter
      if (!m_lucidLoaded) {
        lucidDreamSequence(240000, 5, 5000, 1000,
                           updateArgs->lucidDream.timestamp);
        m_lucidLoaded = true;
        return UPDATE_NEW_DATA;
      }
    }
    break;
  default:
    if (remDuration >= 600000) { // TODO: 600000 needs to ba a parameter
      if (!m_lucidLoaded) {
        lucidDreamSequence(240000, 5, 5000, 1000,
                           updateArgs->lucidDream.timestamp);
        m_lucidLoaded = true;
        return UPDATE_NEW_DATA;
      }
    }
    break;
  }

  return UPDATE_OK;
}

void LucidDreamScenario::lucidDreamSequence(unsigned long length,
                                            unsigned long numberOfActions,
                                            unsigned long actionDuration,
                                            unsigned long actionPeriod,
                                            unsigned long timestamp) {
  ncAtomicInstruction instruction;
  uint8_t intensities[7];
  std::memset(intensities, 63, sizeof(intensities));
  size_t len;
  for (unsigned long i = 0; i < numberOfActions; ++i) {
    device_set_func(instruction.data, &len,
                    DEV_POWER_LED | DEV_RIGHT_RED_LED | DEV_LEFT_RED_LED,
                    FUN_TYPE_BLINK, intensities, actionDuration / 100,
                    actionPeriod / 100, 0);
    instruction.time = timestamp;
    pushInstruction(instruction);
    timestamp += numberOfActions > 1 ? length / (numberOfActions - 1) : 0;
  }
}
