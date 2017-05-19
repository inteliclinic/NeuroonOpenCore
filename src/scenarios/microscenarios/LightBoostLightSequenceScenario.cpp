#include "LightBoostLightSequenceScenario.h"
#include "MicroScenarioSequenceLibrary.h"

MicroScenario::MicroScenarioUpdate
LightBoostLightSequenceScenario::update(bool did_activate,
                                        bool last_instructions) {

  MicroScenarioState state = this->isTriggerActive(kActivationTrigger)
                                 ? MicroScenarioState::WANTS_FOCUS
                                 : MicroScenarioState::IDLE;

  if (last_instructions) {
    // we must deactivate scenario if active
    return {state,
            did_activate ? this->_descendSequence()
                         : std::vector<ncAtomicInstruction>{},
            {}};
  }

  // activation trigger is active and scenarios instruction
  // hasnt been already activated
  if (this->isTriggerActive(kActivationTrigger) && !did_activate) {
    // its time to turn on the lights
    return {state, this->_parametrizedSinusLikeSequence(),
            this->_descendSequence()};
  }
  // if lights are already on
  else if (this->isTriggerActive(kActivationTrigger)) {
    return {state, {}, {}};
  }

  // trigger has just become inactive and scenarios light did activate
  if (this->wasTriggerJustDeactivated(kActivationTrigger) && did_activate) {
    // send turning  off as activation
    return {state, this->_descendSequence(), {}};
  }

  // nothing interesting is happening
  return {state, {}, {}};
}

LightBoostLightSequenceScenario::LightBoostLightSequenceScenario(
    unsigned int ascend_time_ms, unsigned int crest_time_ms,
    unsigned int descend_time_ms, unsigned int trough_time_ms,
    unsigned int duration_ms):
  m_ascendTimeMs(ascend_time_ms),
  m_crestTimeMs(crest_time_ms),
  m_descendTimeMs(descend_time_ms),
  m_troughTimeMs(trough_time_ms),
  m_durationMs(duration_ms)
{}

std::vector<ncAtomicInstruction>
LightBoostLightSequenceScenario::_parametrizedSinusLikeSequence() const {
  return {sequence::parametrizedSinusLikeSequence(m_ascendTimeMs, m_crestTimeMs, m_descendTimeMs, m_troughTimeMs, m_durationMs)};
}

std::vector<ncAtomicInstruction>
LightBoostLightSequenceScenario::_descendSequence() const {
  return {sequence::killSequence()};
}
