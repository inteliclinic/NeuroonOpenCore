/**
 * @file    TriggerScenario.h
 * @Author  Paweł Kaźmierzewski <p.kazmierzewski@inteliclinic.com>
 * @date    April, 2017
 * @brief   Brief description
 *
 * Description
 */

#ifndef TRIGGERSCENARIO_H
#define TRIGGERSCENARIO_H

typedef struct {
}TriggerInput;

class TriggerScenario {
  TriggerScenario();
  virtual ~TriggerScenario() = 0;
  bool checkCondition(TriggerInput input);
};

#endif /* !TRIGGERSCENARIO_H */
