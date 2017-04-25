#include "MacroScenario.h"

ncUpdateOutput MacroScenario::update(ncUnixTimestamp ts,
                                     const ncScenarioInput *inp) {
  this->_current_moment = ts;
  for (auto t : this->_triggers_refresh_list) {
    t->update(inp);
  }

  // prepare copy of prioritized scenarios vector
  auto scenarios = std::vector<ScePrioPair>(this->_scenario_queue.begin(),
                                            this->_scenario_queue.end());
  if (scenarios.empty()) {
    return UPDATE_SCENARIO_FINISHED;
  }

  // finish if all microscenarios are finished
  auto finished = true;
  for (const auto &scep : scenarios) {
    if (!scep.first->isFinished()) {
      finished = false;
      break;
    }
  }
  if (finished) {
    return UPDATE_SCENARIO_FINISHED;
  }

  // get first active scenario with highest priority
  MicroScenario *sc = nullptr;
  while (scenarios.size()) {
    sc = scenarios.front().first;
    std::pop_heap(scenarios.begin(), scenarios.end(), Cmp());
    scenarios.pop_back();
    if (sc->isActive())
      break;
  }

  // this will hold scenarios that may provide instructions during these updates
  MicroScenario *first_sc, *snd_sc;

  // if no scenario provide instructions return update_ok
  ncUpdateOutput first_ret = UPDATE_OK;
  ncUpdateOutput snd_ret = UPDATE_OK;

  // no scenario is active or same scenario is active again
  if (!sc || this->_last_active_scenario == sc) {
    // refresh scenarios
    for (const auto &scep : scenarios) {
      auto outp = scep.first->refresh(ts);
      // if theres was active scenario capture its output
      // and remember as instruction provider
      if (sc && scep.first == this->_last_active_scenario) {
        first_ret = outp;
        first_sc = scep.first;
      }
    }
  }
  // there is an active scenario, and it is different than
  // the one at the previous update
  else {
    for (const auto &scep : scenarios) {
      // non-active scenarios should be muted if not muted already
      if (sc != scep.first) {
        auto outp = scep.first->refresh(ts, MicroScenarioControl::MUTE);
        if (scep.first == this->_last_active_scenario) {
          // this one was active last time - it may provide
          // desactivation instructions - remember it
          first_ret = outp;
          first_sc = scep.first;
        }
      }
      // active scenario should be unmuted if muted,
      // and remembere for instructions
      else {
        snd_ret = sc->isMuted()
                      ? sc->refresh(ts, MicroScenarioControl::UNMUTE)
                      : sc->refresh(ts, MicroScenarioControl::REFRESH);
        snd_sc = sc;
      }
    }
  }

  this->_last_active_scenario = sc;

  // There was a scenario error, better to just return
  // that it has happened.
  if(first_ret == UPDATE_ERROR || snd_ret == UPDATE_ERROR){
    return UPDATE_ERROR;
  }

  // clear not yet used instructions
  this->clearInstructions();

  std::vector<ncAtomicInstruction> instrs;
  while(first_sc->availableMaskInstruction()){
    instrs.push_back(first_sc->getNextInstruction());
  }

  // insert desactivation instructions right now, but normal instructions
  // as intended
  this->pushInstructions(instrs, !snd_sc ? InstructionInsertMode::NOW : InstructionInsertMode::ABSOLUTE);

  // insert activation instructions after desactivation
  instrs.clear();
  while(snd_sc->availableMaskInstruction()){
    instrs.push_back(snd_sc->getNextInstruction());
  }
  this->pushInstructions(instrs, InstructionInsertMode::LAST);

  // return correct flag
  if(first_ret == UPDATE_NEW_DATA || first_ret == UPDATE_NEW_DATA ){
    return UPDATE_NEW_DATA;
  }

  // assumption: the only viable return flag is UPDATE_OK
  return UPDATE_OK;
}
