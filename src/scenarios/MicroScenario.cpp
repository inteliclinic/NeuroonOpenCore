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

ncUpdateOutput MicroScenario::refresh(ncUnixTimestamp ts,
                                      MicroScenarioControl control, int ms) {

  // scenario is finished
  if (this->_is_finished) {
    return UPDATE_SCENARIO_FINISHED;
  }

  // update time
  this->_current_moment = ts;

  // check if scenario is muted, and should be unmuted already
  if (this->_muted_till > 0 && this->_muted_till < this->currentMoment()) {
    this->_muted_till = 0;
  }

  // execute control command
  switch (control) {
  case MS_REFRESH: {
    return this->isMuted() ? UPDATE_OK : this->_refreshOnActiveTriggers();
  }
  case MS_UNMUTE: {
    // unconditionally unmute
    this->_muted_till = 0;
    return this->_refreshOnActiveTriggers();
  }
  case MS_MUTE: {
    auto ret = this->onMute();
    // mute for given number of milis
    this->_muted_till = this->currentMoment() + ms;
    return ret;
  }
  case MS_FINISH:
    return this->onFinish();
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
