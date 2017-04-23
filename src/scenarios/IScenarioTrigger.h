/**
 * @file    IScenarioTrigger.h
 * @Author  Paweł Kaźmierzewski <p.kazmierzewski@inteliclinic.com>
 * @date    April, 2017
 * @brief   Brief description
 *
 * Description
 */

#ifndef ISCENARIOTRIGGER_H
#define ISCENARIOTRIGGER_H

class IScenarioTrigger {
public:
  virtual bool isActive() const = 0;
};

#endif /* !ISCENARIOTRIGGER_H */
