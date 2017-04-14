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

class LucidDreamScenario: SleepScenario{
  public:
    ncLucidPulsesIntensity m_startingIntensity;
    ncLucidRemStabilityTreshold m_remStabilityTreshold;

    unsigned long m_remCounter;
    unsigned long m_remStartTimestamp;
    bool m_remDetected;
    bool m_remCounted;

    bool m_lucidLoaded;

    LucidDreamScenario();
    LucidDreamScenario(const ncScenarioInitArgs *args);

    void lucidDreamSequence(unsigned long length, unsigned long numberOfActions,
        unsigned long actionDuration, unsigned long actionPeriod, unsigned long timestamp);
    ncUpdateOutput update(const ncScenarioInput *updateArgs);
};

#endif /* !LUCIDDREAMSCENARIO_H */
