/**
 * @file    SleepScenario.h
 * @Author  Paweł Kaźmierzewski <p.kazmierzewski@inteliclinic.com>
 * @date    April, 2017
 * @brief   Brief description
 *
 * Description
 */

#ifndef SLEEPSCENARIO_H
#define SLEEPSCENARIO_H

#include "MacroScenario.h"


class SleepScenario : public MacroScenario {
private:
  const int kContactDetectionTresholdMs = 5000;

protected:

  const int kContactTestPriority = 100;
  const int kHardWakeupPriority = 1000;
  const int kSmartWakeupPriority = 50;
  const int kSunrisePriority = 30;

public:
  SleepScenario(const ncScenarioInitArgs *args);
  virtual ~SleepScenario() {}
};

#endif /* !SLEEPSCENARIO_H */
