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
  m_remCounted(false), m_lucidLoaded(false) {

    auto rem_detected_trigger = LucidDreamScenario::moveToShared(

        RemDetectedTrigger::WithAdapter<const ncScenarioInput *>(
          std::unique_ptr<RemDetectedTrigger>(
            new RemDetectedTrigger(m_kRemTresholdMs, m_kRemFirstOffset, m_kRemSecondOffset)),
          [](const ncScenarioInput *inp){
          return std::make_pair(
              inp->commonInput.currentTime,
              inp->scenarioSpecific.lucidDream.currentSleep_stage);
          }
          )
        );

    this->addTriggersToRefreshList({rem_detected_trigger});

  }
