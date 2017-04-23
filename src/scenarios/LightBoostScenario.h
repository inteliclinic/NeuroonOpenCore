/**
 * @file    LightBoostScenario.h
 * @Author  Michał Adamczyk <m.adamczyk@inteliclinic.com>
 * @date    April, 2017
 * @brief   Brief description
 *
 * Description
 */

#ifndef LIGHTBOOSTSCENARIO_H
#define LIGHTBOOSTSCENARIO_H

#include "SleepScenario.h"

class LightBoostScenario: SleepScenario{
public:
  Lightboostscenario(const ncScenarioInitArgs *args);
  ncUpdateOutput update(const ncScenarioInput *updateArgs);
private:
  ncLucidPulsesIntensity m_startingIntensity;
  ncLucidRemStabilityTreshold m_remStabilityTreshold;
  unsigned long m_remCounter;
  unsigned long m_remStartTimestamp;
  bool m_remDetected;
  bool m_remCounted;
  bool m_lucidLoaded;
  void lucidDreamSequence(unsigned long length, unsigned long numberOfActions,
                          unsigned long actionDuration, unsigned long actionPeriod, unsigned long timestamp);
};

#endif /* !LIGHTBOOSTSCENARIO_H */
