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

typedef enum {

} WakeUpType;

class SleepScenario : public MacroScenario {

public:
  SleepScenario(const ncScenarioInitArgs *args);
  virtual ~SleepScenario() {}

  struct {
    unsigned long wakeUpDownCounter;
    WakeUpType wakeUp;
  } wakeUpParameters;

};

#endif /* !SLEEPSCENARIO_H */
