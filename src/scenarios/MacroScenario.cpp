#include "MacroScenario.h"
#include "logger.h"

ScePrioPair MacroScenario::getHighestPriorityScenario(
    std::vector<ScePrioPair> scenarios) const {
  ScePrioPair scep;
  while (scenarios.size()) {
    // get highest priority scenario
    scep = scenarios.front();

    // keep heap structure
    std::pop_heap(scenarios.begin(), scenarios.end(), Chronological());
    scenarios.pop_back();

    // does it want focus
    if (scep.first->getState() == MicroScenarioState::WANTS_FOCUS)
      break;
  }
  // return it
  return scep;
}

ncUpdateOutput MacroScenario::update(const ncScenarioInput *inp) {

  // update triggers
  auto ts = inp->commonInput.currentTime;
  this->_current_moment = ts;
  for (auto t : this->_triggers_refresh_list) {
    t->update(inp);
  }

  // shorter local name and make it const
  const auto &scenarios = std::vector<ScePrioPair>(
      this->_scenario_queue.begin(), this->_scenario_queue.end());
  if (scenarios.empty()) {
    return UPDATE_SCENARIO_FINISHED;
  }

  // update microscenarios
  for (const auto &scep : scenarios) {
    scep.first->refresh(ts);
  }

  switch (m_mute_state.getMuteOperation(ts)) {
  case MuteOperation::SHOULD_MUTE: {
    // stoping current scenario
    auto instrs = this->_recently_used_scenario->stopUse();
    this->pushInstructions(instrs, InstructionInsertMode::NOW);
    return UPDATE_NEW_DATA;
  }
  case MuteOperation::SHOULD_UNMUTE: {
    break;
  }
  case MuteOperation::MUTED: {
    return UPDATE_OK;
  }
  case MuteOperation::UNMUTED: {
    break;
  }
  default:
    LOG(ERROR) << "Invalid mute operation - #wtf";
    throw std::logic_error("Invalid mute operation - #wtf");
    break;
  }

  // finish if all microscenarios are finished
  auto finished = true;
  for (const auto &scep : scenarios) {
    if (scep.first->getState() != MicroScenarioState::FINISHED) {
      finished = false;
      break;
    }
  }
  if (finished) {
    return UPDATE_SCENARIO_FINISHED;
  }

  // get first scenario with highest priority
  // that wants focus of the macroscenario
  auto sc = this->getHighestPriorityScenario(scenarios).first;

  bool new_instructions = false;

  // same scenario will be used again
  if (this->_recently_used_scenario &&
      // [TODO][WARNING] i am not sure if this comparison works as expected
      (!sc || this->_recently_used_scenario == sc.get())) {
    // in that case just use instructions from recently used
    auto instrs = this->_recently_used_scenario->use();

    // if there are new instructions
    new_instructions = instrs.empty();

    // clear not yet used instructions
    if (new_instructions)
      this->clearInstructions();

    // and push new
    this->pushInstructions(instrs, InstructionInsertMode::NOW);
  }

  // there is active scenario, and it is different than
  // the one at the previous update
  else {
    // get new instructions from scenarios
    std::vector<ncAtomicInstruction> instrs = {};
    if(this->_recently_used_scenario){
      instrs = this->_recently_used_scenario->stopUse();
    }
    auto instrs2 = sc->use();

    // if there are new instructions
    new_instructions = instrs.empty() && instrs2.empty();

    // clear not yet used instructions
    if (new_instructions)
      this->clearInstructions();

    // and push new
    this->pushInstructions(instrs, InstructionInsertMode::NOW);
    this->pushInstructions(instrs2, InstructionInsertMode::LAST);

    // update recently used scenario
    this->_recently_used_scenario = sc.get();
  }

  // return the right flag
  // [TODO][WARNING] i am not sure if this is a right thing
  return new_instructions ? UPDATE_NEW_DATA : UPDATE_OK;
}
