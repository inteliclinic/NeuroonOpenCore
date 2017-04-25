/**
 * @file    MicroScenario.cpp
 * @Author  Michał Adamczyk <m.adamczyk@inteliclinic.com>
 * @Author  Paweł Kaźmierzewski <p.kazmierzewski@inteliclinic.com>
 * @date    April, 2017
 * @brief   Brief description
 *
 * Description
 */

#include "MicroScenario.h"
#include <algorithm>
#include <cstring>

bool MicroScenario::isActive() const {
  for (const auto &p : this->_triggers) {
    if (p.first->isActive())
      return true;
  }
  return false;
}

ncUpdateOutput MicroScenario::refresh(ncUnixTimestamp ts,
                                      MicroScenarioControl control, int ms) {

  // scenario is finished
  if (this->_is_finished) {
    return UPDATE_SCENARIO_FINISHED;
  }

  // update time
  this->_current_moment = ts;

  // check if scenario is muted, and should be unmuted already
  if (this->_muted_till > 1 && this->_muted_till < this->currentMoment()) {
    this->_muted_till = 0;
  }

  // execute control command
  switch (control) {
  case MicroScenarioControl::REFRESH: {
    return this->isMuted() ? UPDATE_OK : this->_refreshOnActiveTriggers();
  }
  case MicroScenarioControl::UNMUTE: {
    // unconditionally unmute
    this->_muted_till = 0;
    return this->_refreshOnActiveTriggers();
  }
  case MicroScenarioControl::MUTE: {
    // already muted, skip
    if (this->_muted_till > 0)
      return UPDATE_OK;
    auto ret = this->onMute();
    // mute forever
    if (ms == 0) {
      this->_muted_till = 1;
    }
    // mute for a given numer of millis
    else {
      this->_muted_till = this->currentMoment() + ms;
    }
    return ret;
  }
  case MicroScenarioControl::FINISH:
    auto ret = this->onFinish();
    this->_is_finished = true;
    return ret;
  }
}

ncUpdateOutput MicroScenario::_refreshOnActiveTriggers() {
  std::set<int> active_triggers;
  for (const auto &kp : this->_triggers) {
    if (kp.first->isActive()) {
      active_triggers.insert(kp.second);
    }
  }
  return this->go(active_triggers);
}

void MicroScenario::installTriggerForKey(const IScenarioTrigger *t, Key k) {
  this->_triggers[t] = k;
}

void MicroScenario::uninstallTrigger(IScenarioTrigger *t) {
  this->_triggers.erase(t);
}
