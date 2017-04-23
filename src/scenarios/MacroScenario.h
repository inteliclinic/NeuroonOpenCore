/**
 * @file    MacroScenario.h
 * @Author  Paweł Kaźmierzewski <p.kazmierzewski@inteliclinic.com>
 * @date    April, 2017
 * @brief   Brief description
 *
 * Description
 */

#ifndef MACROSCENARIO_H
#define MACROSCENARIO_H

#include "MicroScenario.h"
#include "Scenario.h"
#include "ScenarioTrigger.h"

#include <map>

class MacroScenario : public Scenario{
public:

  MacroScenario(const ncScenarioInitArgs* args) {}
  virtual ncUpdateOutput update(const ncScenarioInput *) = 0;
  virtual ~MacroScenario() {}

};

#endif /* !MACROSCENARIO_H */
