#include "ContactTestScenario.h"
#include <algorithm>

const std::vector<ncAtomicInstruction> &
ContactTestScenario::_getLightsOnInstructions() {
  if (this->_lights_on_instructions.empty()) {
    // [TODO] prepare actual instructions and set it
  }
  return this->_lights_on_instructions;
}

const std::vector<ncAtomicInstruction> &
ContactTestScenario::_getLightsOffInstructions() {
  if (this->_lights_off_instructions.empty()) {
    // [TODO] prepare actual instructions and set it
  }
  return this->_lights_off_instructions;
}

ncUpdateOutput ContactTestScenario::go(const std::set<Key> &triggers) {

  // after timeout scenario is not to be used
  if (triggers.find(kDontWakeTimer) != triggers.end()) {
    return this->onFinish();
  }
  // if signal is lost
  if (triggers.find(kSignalLost) != triggers.end()) {

    // lights are already on and not planned to turn off
    if (this->_lights_would_turn_off_at == 0 &&
        // this should be true, unlees on start
        this->_lights_would_turn_on_at != 0) {
      return UPDATE_OK;
    }
    // lights are already on but are planned to be turn off
    else if (this->_lights_would_turn_off_at > this->currentMoment()) {
      // in this case cancel turning off
      this->clearInstructions();
      return UPDATE_NEW_DATA;
    } else {
      // we can assume lights are already off
      this->_lights_would_turn_off_at = 0;
      // lights are scheduled on
      if (this->_lights_would_turn_on_at > this->currentMoment()) {
        return UPDATE_OK;
      } else {
        // lights should be scheduled
        auto inss = this->_getLightsOnInstructions();
        this->pushInstructions(inss);
        this->_lights_would_turn_on_at =
            this->currentMoment() + 1 + inss[0].time;
        return UPDATE_NEW_DATA;
      }
    }
  }
  // signal is not lost
  else {
    // lights are already off and not planned to turn on
    if (this->_lights_would_turn_on_at == 0
        // this should be true
        && this->_lights_would_turn_off_at != 0) {
      return UPDATE_OK;
    }
    // lights are already off but are planned to be turn on
    else if (this->_lights_would_turn_on_at > this->currentMoment()) {
      // in this case cancel turning on
      this->clearInstructions();
      return UPDATE_NEW_DATA;
    } else {
      // we can assume lights are already on
      this->_lights_would_turn_on_at = 0;
      // lights are scheduled off
      if (this->_lights_would_turn_off_at > this->currentMoment()) {
        return UPDATE_OK;
      } else {
        // lights off should be scheduled
        auto inss = this->_getLightsOffInstructions();
        this->pushInstructions(inss);
        this->_lights_would_turn_off_at =
            this->currentMoment() + 1 + inss[0].time;
        return UPDATE_NEW_DATA;
      }
    }
  }
}

ncUpdateOutput ContactTestScenario::onFinish() {

  // turn off the lights if they are turned on, or cancel if planned ot be
  // turned on
  // [TODO]
  return UPDATE_OK;
}

ncUpdateOutput ContactTestScenario::onMute() {

  // turn off the lights if they are turned on, or cancel if planned ot be
  // turned on
  // [TODO]
  return UPDATE_OK;
}
