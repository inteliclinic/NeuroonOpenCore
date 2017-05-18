/**
 * @file    LostContactTrigger.cpp
 * @Author  Michał Adamczyk <m.adamczyk@inteliclinic.com>
 * @date    April, 2017
 * @brief   Brief description
 *
 * Simple trigger that will be active when the electrode contact is lost.
 */

#include "LostContactTrigger.h"

LostContactTrigger::LostContactTrigger(
    int switch_treshold_ms, ncUnixTimestamp trigger_timeout_timestamp,
    ncSignalQuality lost_contact_treshold)
    : _switch_treshold_ms(switch_treshold_ms),
      _trigger_timeout_timestamp(trigger_timeout_timestamp),
      _lost_contact_treshold(lost_contact_treshold) {}

bool LostContactTrigger::isActive() const { return _active; }

void LostContactTrigger::update(const lctInput &inp) {
  auto ts = inp.first;
  auto sq = inp.second;
  bool now_below_treshold = sq <= this->_lost_contact_treshold;

  // trigger is timed out
  if (this->_trigger_timeout_timestamp != 0 &&
      this->_trigger_timeout_timestamp < ts) {
    this->_active = false;
    return;
  }

  // havent been updated yet
  if (this->_start == 0) {
    this->_start = ts;
    this->_last_contact_change_ts = ts;
    this->_last_contact_change_below_treshold =
        sq <= this->_lost_contact_treshold;
  }

  // there is no treshold buffer, so just check signal quality treshold
  if (this->_switch_treshold_ms == 0) {
    this->_active = now_below_treshold;
    return;
  }

  // if signal is not lost and current status is below treshold
  if (!this->_active && now_below_treshold) {
    // we approached signal lost treshold recently
    if (this->_last_contact_change_below_treshold &&
        // enough to trigger lost signal
        ts - this->_last_contact_change_ts >= this->_switch_treshold_ms) {
      this->_active = true;
    }
  }
  // if signal is lost and current status is over treshold
  else if (this->_active && !now_below_treshold) {
    // we approached signal lost treshold recently
    if (!this->_last_contact_change_below_treshold &&
        // enough to deactivate trigger
        ts - this->_last_contact_change_ts >= this->_switch_treshold_ms) {
      this->_active = false;
    }
  }
  // update last status
  if (this->_last_contact_change_below_treshold != now_below_treshold) {
    this->_last_contact_change_below_treshold = now_below_treshold;
    this->_last_contact_change_ts = ts;
  }
}
