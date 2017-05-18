/**
 * @file    IScenarioTrigger.h
 * @Author  Michał Adamczyk <m.adamczyk@inteliclinic.com>
 * @date    April, 2017
 * @brief   Abstract scenario-facing interface for a scenario trigger.
 *
 */

#ifndef ISCENARIOTRIGGER_H
#define ISCENARIOTRIGGER_H

class IScenarioTrigger {
public:
  virtual bool isActive() const = 0;
  virtual ~IScenarioTrigger() {}
};

#endif /* !ISCENARIOTRIGGER_H */
