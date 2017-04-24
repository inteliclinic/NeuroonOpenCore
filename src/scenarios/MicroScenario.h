/**
 * @file    MicroScenario.h
 * @Author  Michał Adamczyk <m.adamczyk@inteliclinic.com>
 * @date    April, 2017
 * @brief   Provides an interface for using trigger-based micro-scenarios.
 *
 * Description [TODO]
 */

#ifndef MICROSCENARIO_H
#define MICROSCENARIO_H

#include "IScenarioTrigger.h"
#include "NeuroonMaskScenariosApi.h"
#include "Scenario.h"
#include <map>
#include <queue>
#include <set>

typedef int Key;

typedef enum{
  MS_REFRESH,
  MS_UNMUTE,
  MS_MUTE,
  MS_FINISH,
}MicroScenarioControl;

class MicroScenario : public Scenario {
public:
  ncUpdateOutput refresh(ncUnixTimestamp ts, MicroScenarioControl control=MS_REFRESH, int ms=1e8);
  void uninstallTrigger(IScenarioTrigger *);
  bool isMuted() const {return this->_muted_till !=0;}

private:
  bool _is_finished = false;
  ncUnixTimestamp _muted_till = 0;
  ncUnixTimestamp _current_moment = 0;
  std::map<const IScenarioTrigger *, Key> _triggers;
  ncUpdateOutput _refreshOnActiveTriggers();

protected:
  void installTriggerForKey(const IScenarioTrigger *, Key);
  virtual ncUpdateOutput go(const std::set<Key> &) = 0;
  virtual ncUpdateOutput onMute() = 0;
  virtual ncUpdateOutput onFinish() = 0;
  virtual ncUnixTimestamp currentMoment() const override { return _current_moment; }
  void finishScenario() { this->_is_finished = true; }

  };

#endif /* !MICROSCENARIO_H */
