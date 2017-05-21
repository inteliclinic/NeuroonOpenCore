#include "MicroScenarioSequenceLibrary.h"
#include "HardWakeupMicroScenario.h"

HardWakeupMicroScenario::HardWakeupMicroScenario(ncWakeUpSequenceIntensity intensity):
  m_activationRegistered(false),
  m_intensity(intensity) {}

HardWakeupMicroScenario::MicroScenarioUpdate
HardWakeupMicroScenario::update(bool did_activate, bool last_instructions) {

  /* tirgger fired sending sequence */
  if(did_activate && !m_activationRegistered){
    m_activationRegistered = true;
    return {MicroScenarioState::WANTS_FOCUS, {
      sequence::wakeUpSequence(static_cast<wakeUpSequenceIntensity>(m_intensity))}, {}};
  }

  /* clean sequnce */
  if(last_instructions){
    if(m_activationRegistered){
      return {MicroScenarioState::WANTS_FOCUS, {sequence::killSequence()}, {}};
    }
    else{
      return {MicroScenarioState::FINISHED, {}, {}};
    }
  }

  if(m_activationRegistered && !did_activate){
    m_activationRegistered = false;
    return {MicroScenarioState::WANTS_FOCUS, {
      sequence::wakeUpSequence(wakeUpSequenceIntensity::WU_ALARM_OFF)}, {}};
  }

  return {MicroScenarioState::IDLE, {}, {}};
}
