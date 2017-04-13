/**
 * @file    SleepScenario.cpp
 * @Author  Paweł Kaźmierzewski <p.kazmierzewski@inteliclinic.com>
 * @date    April, 2017
 * @brief   Brief description
 *
 * Description
 */

#include "SleepScenario.h"

SleepScenario::SleepScenario(){
}

SleepScenario::SleepScenario(const ncScenarioInitArgs *args){
}

ncUpdateOutput SleepScenario::update(const ncScenarioInput *scenarioInput){
  return UPDATE_OK;
}
