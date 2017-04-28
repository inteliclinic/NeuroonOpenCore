#include "ContactTestScenario.h"
#include <algorithm>

MicroScenario::MicroScenarioUpdate
ContactTestScenario::update(bool did_activate, bool last_instructions) {

  MicroScenarioState state = this->isTriggerActive(kSignalLost)
                                 ? MicroScenarioState::WANTS_FOCUS
                                 : MicroScenarioState::IDLE;

  if (last_instructions) {
    // we must deactivate scenario if active
    return {state,
            did_activate ? this->_getLightsOffInstructions()
                         : std::vector<ncAtomicInstruction>{},
            {}};
  }

  // signal is lost and scenarios instruction hasnt been
  // already activated
  if (this->isTriggerActive(kSignalLost) && !did_activate) {
    // its time to turn on the lights
    return {state, this->_getLightsOnInstructions(),
            this->_getLightsOffInstructions()};
  }
  // if lights are already on
  else if (this->isTriggerActive(kSignalLost)) {
    return {state, {}, {}};
  }

  // signal was just established and scenarios light did activate
  if (this->wasTriggerJustDeactivated(kSignalLost) && did_activate) {
    // send turning  off as activation
    return {state, this->_getLightsOffInstructions(), {}};
  }

  // nothing interesting is happening
  return {state, {}, {}};
}

const std::vector<ncAtomicInstruction> &
ContactTestScenario::_getLightsOnInstructions() {
  if (this->_lights_on_instructions.empty()) {
    // [TODO] prepare actual instructions and set it
    //
    // Sekwencja rozpalajaca swiatla ktore wskazuja na brak sygnalu
    // moze byc liniowe narastanie, jakiegos pomaranczowego koloru
    // przy osiagnieciu maksimum moze pulsowac
  }
  return this->_lights_on_instructions;
}

const std::vector<ncAtomicInstruction> &
ContactTestScenario::_getLightsOffInstructions() {
  if (this->_lights_off_instructions.empty()) {
    // [TODO] prepare actual instructions and set it
    //
    // Sekwencja gasząca światełka sygnalizujące brak sygnalu
  }
  return this->_lights_off_instructions;
}
