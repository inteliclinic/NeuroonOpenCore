/**
 * @file    LostContactTrigger.h
 * @Author  Michał Adamczyk <m.adamczyk@inteliclinic.com>
 * @date    April, 2017
 * @brief   Brief description
 *
 * Simple trigger that will be active when the electrode contact is lost.
 * [TODO] dokumentacja
 */

#ifndef LOSTCONTACTTRIGGER_H
#define LOSTCONTACTTRIGGER_H


#include "NeuroonApiCommons.h"
#include "ScenarioTrigger.h"

typedef std::pair<ncUnixTimestamp, ncSignalQuality> ltcInput;

class LostContactTrigger : public ScenarioTrigger<ltcInput> {

  LostContactTrigger(int switch_treshold_ms,
                     ncUnixTimestamp trigger_timeout_timestamp,
                     ncSignalQuality lost_contact_treshold);

  void update(const ltcInput &inp) override;

  bool isActive() const override;

private:
  ncUnixTimestamp _start = 0;
  int _switch_treshold_ms = 0;
  ncUnixTimestamp _trigger_timeout_timestamp = 0;

  ncSignalQuality _lost_contact_treshold = NO_SIGNAL;
  bool _last_contact_change_below_treshold = NO_SIGNAL;
  ncUnixTimestamp _last_contact_change_ts = 0;

  bool _active = false;
};

#endif /* !LOSTCONTACTTRIGGER_H */
