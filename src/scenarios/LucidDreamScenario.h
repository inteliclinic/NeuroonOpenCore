/**
 * @file    LucidDreamScenario.h
 * @Author  Paweł Kaźmierzewski <p.kazmierzewski@inteliclinic.com>
 * @date    April, 2017
 * @brief   Brief description
 *
 * Description
 */

#ifndef LUCIDDREAMSCENARIO_H
#define LUCIDDREAMSCENARIO_H

#include "SleepScenario.h"
#include "RemDetectedTrigger.h"

// [TODO] warning: previosly declared as struct
class LucidDreamScenario : public SleepScenario {
public:
  LucidDreamScenario(const ncScenarioInitArgs *args);
  // ncUpdateOutput update(const ncScenarioInput *updateArgs) override;

protected:
private:
  const ncUnixTimestamp m_kRemTresholdMs = 180000;
  const ncUnixTimestamp m_kRemFirstOffset = 300000;
  const ncUnixTimestamp m_kRemSecondOffset = 600000;

  ncLucidPulsesIntensity m_startingIntensity;
  ncLucidRemStabilityTreshold m_remStabilityTreshold;
  unsigned long m_remCounter;
  unsigned long m_remStartTimestamp;
  bool m_remDetected;
  bool m_remCounted;
  bool m_lucidLoaded;
  void lucidDreamSequence(unsigned long length, unsigned long numberOfActions,
                          unsigned long actionDuration,
                          unsigned long actionPeriod, unsigned long timestamp);
};

#endif /* !LUCIDDREAMSCENARIO_H */
