#include "MicroScenario.h"
#include <algorithm>
#include <cstring>

std::vector<ncAtomicInstruction> MicroScenario::use() {
  if (_did_activate) {
    // theres nothing to be done now
    return {};
  }
  // we assume that the activation instructions will be used
  // therefore we need new deactivation instruction set
  _deactivation = _next_deactivation;
  _did_activate = true;
  _did_deactivate = false;

  // if this is the last set of instructions we may
  // mark scenario as finished
  if(_will_be_finished){
    _state = MicroScenarioState::FINISHED;
  }
  // return the activation instruction set
  return _activation;
}

std::vector<ncAtomicInstruction> MicroScenario::stopUse() {
  if (_did_deactivate) {
    // this deactivation instruction set was already used
    return {};
  }

  // we assume that the deactivation instructions will be used
  // therefore we need to mark scenario as not activated
  _did_activate = false;
  _did_deactivate = true;

  // return the deactivation instruction set
  return _deactivation;
}


bool MicroScenario::isTriggerActive(Key k) const{
  return this->_currently_active_triggers.find(k) != this->_currently_active_triggers.end();
}

bool MicroScenario::wasTriggerJustActivated(Key k) const{
  return (this->_currently_active_triggers.find(k) != this->_currently_active_triggers.end() &&
          this->_previously_active_triggers.find(k) == this->_previously_active_triggers.end());
}

bool MicroScenario::wasTriggerJustDeactivated(Key k) const{
  return (this->_currently_active_triggers.find(k) == this->_currently_active_triggers.end() &&
          this->_previously_active_triggers.find(k) != this->_previously_active_triggers.end());
}

void MicroScenario::refresh(ncUnixTimestamp ts) {
  // update current moment and currently active triggers
  this->_current_moment = ts;
  this->_currently_active_triggers = this->_getCurrentlyActiveTriggers();

  // kill trigger is active
  if(this->_kill_trigger->isActive()){
    this->_will_be_finished = true;
  }

  // get instructions from concrete scenario implementation
  auto new_state = this->update(this->_did_activate, this->_will_be_finished);

  // update previously active triggers
  this->_previously_active_triggers = this->_currently_active_triggers;

  // update state
  this->_state = new_state.state_update;
  if(!new_state.activation.empty()){
    // there are new instructions which means that
    // the scenario did not active them yet.
    _did_activate = false;
  }
  _activation = new_state.activation;
  _deactivation = new_state.deactivation;
}

std::set<Key> MicroScenario::_getCurrentlyActiveTriggers() const{
  std::set<Key> ret = {};
  for(const auto & trikeypair : this->_trigger_to_key){
    if(trikeypair.first->isActive()){
      ret.insert(trikeypair.second);
    }
  }
  return ret;
}

ncAtomicInstruction MicroScenario::createAtomicInstruction(unsigned int time, char (&data)[MASK_INSTRUCTION_LENGTH]){
  ncAtomicInstruction atomic_instruction;
  atomic_instruction.time = time;
  std::memcpy(atomic_instruction.data, data, sizeof(data));
  return atomic_instruction;
}

