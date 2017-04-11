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

#include "BaseScenario.h"

struct LucidDreamScenario:BaseScenario{
  ncLucidPulsesIntensity startingIntensity;
  ncLucidRemStabilityTreshold remStabilityTreshold;

  unsigned long remCounter;
  unsigned long remStartTimestamp;
  bool remDetected;
  bool remCounted;

  bool lucidLoaded;

  LucidDreamScenario();
  void lucidDreamSequence(unsigned long length, unsigned long numberOfActions,
      unsigned long actionDuration, unsigned long actionPeriod, unsigned long timestamp);
  bool update(const ScenarioInput *updateArgs);

};

#endif /* !LUCIDDREAMSCENARIO_H */
