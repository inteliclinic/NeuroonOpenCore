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
  MacroScenario(args), m_scenario(scenario), m_prevTime(0){
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
  auto _loadNextSeq = (inp->commonInput.currentTime - m_prevTime)>=m_deltaTime;

  if (_loadNextSeq){
    m_prevTime = inp->commonInput.currentTime;
    auto _retVal = ++m_seqCounter == kNumberOfSeq ? ncUpdateOutput::UPDATE_SCENARIO_FINISHED : ncUpdateOutput::UPDATE_NEW_DATA;
    if(_retVal == ncUpdateOutput::UPDATE_NEW_DATA)
      switch(m_scenario){
        case SCENARIO_LUCIDDREAM:
          this->pushInstructions(msequence::lucidDreamMocSequence(this->currentMoment()));
          break;
        case SCENARIO_LIGHTBOOST:
          this->pushInstructions(msequence::lightBoostMocSequence(this->currentMoment()));
          break;
        case SCENARIO_SLEEP:
          this->pushInstructions(msequence::sleepMocSequence(this->currentMoment()));
          break;
        case SCENARIO_POWERNAP:
          this->pushInstructions(msequence::powerNapMocSequence(this->currentMoment()));
          break;
        case SCENARIO_CIRCADIANRHYTHM:
          this->pushInstructions(msequence::circadianRythmMocSequence(this->currentMoment()));
          break;
      }
    return _retVal;
  }
  return ncUpdateOutput::UPDATE_OK;
}

