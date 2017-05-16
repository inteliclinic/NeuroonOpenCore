#include "LucidDreamLightsScenario.h"

LucidDreamLightsScenario::LucidDreamLightsScenario() :
  m_sequenceGenerated(false),
  m_trigerAcknowledged(false) {}

LucidDreamLightsScenario::MicroScenarioUpdate
LucidDreamLightsScenario::update(bool did_activate, bool last_instructions) {

  if (last_instructions){
    return {MicroScenarioState::WANTS_FOCUS, this->_desequence(), {}};
  }

  if (this->isTriggerActive(TriggersIDs::REMPHASE)){
    m_trigerAcknowledged = true;
    if (!m_sequenceGenerated){
      m_sequenceGenerated = true;
      return {MicroScenarioState::WANTS_FOCUS,this->LucidSequence(), {}};
    }
  }
  else{
    m_trigerAcknowledged = false;
  }

  return {MicroScenarioState::IDLE, {}, {}};
}


std::vector<ncAtomicInstruction> LucidDreamLightsScenario::LucidSequence(){
  return {};
}


std::vector<ncAtomicInstruction> LucidDreamLightsScenario::_desequence(){
  return {};
}

