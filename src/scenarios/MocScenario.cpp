/**
 * @file    MocScenario.cpp
 * @Author  Paweł Kaźmierzewski <p.kazmierzewski@inteliclinic.com>
 * @date    May, 2017
 * @brief   Brief description
 *
 * Description
 */

#include "MocScenario.h"
#include "MicroScenarioSequenceLibrary.h"

MocScenario::MocScenario(const ncScenarioInitArgs *args, ncScenarioType scenario):
  m_scenario(scenario){
  switch (m_scenario){
    case SCENARIO_LUCIDDREAM:
      m_deltaTime = kLucidDelta;
      break;
    case SCENARIO_LIGHTBOOST:
      m_deltaTime = kLightBoostDelta;
      break;
    case SCENARIO_SLEEP:
      m_deltaTime = kSleepDelta;
      break;
    case SCENARIO_POWERNAP:
      m_deltaTime = kPowerNapDelta;
      break;
    case SCENARIO_CIRCADIANRHYTHM:
      m_deltaTime = kCircadianDelta;
      break;
  }
}

ncUpdateOutput MocScenario::update(const ncScenarioInput *inp){
  auto _timestamp = inp->commonInput.currentTime;
  auto _loadNextSeq = (_timestamp - m_prevTime)>=m_deltaTime;

  if (_loadNextSeq){
    auto _retVal = ++m_seqCounter == kNumberOfSeq ? UPDATE_SCENARIO_FINISHED : UPDATE_NEW_DATA;
    if(_retVal == UPDATE_NEW_DATA)
      switch(m_scenario){
        case SCENARIO_LUCIDDREAM:
          /* [TODO] insert lucid MOC sequence */
          // this->pushInstructions(/* [TODO] */);
          break;
        case SCENARIO_LIGHTBOOST:
          /* [TODO] insert LB MOC sequence */
          // this->pushInstructions(/* [TODO] */);
          break;
        case SCENARIO_SLEEP:
          /* [TODO] insert lucid MOC sequence */
          // this->pushInstructions(/* [TODO] */);
          break;
        case SCENARIO_POWERNAP:
          /* [TODO] insert lucid MOC sequence */
          // this->pushInstructions(/* [TODO] */);
          break;
        case SCENARIO_CIRCADIANRHYTHM:
          /* [TODO] insert lucid MOC sequence */
          // this->pushInstructions(/* [TODO] */);
          break;
      }
    return _retVal;
  }
  return UPDATE_OK;
}

