/**
 * @file    MacroScenario.h
 * @Author  Paweł Kaźmierzewski <p.kazmierzewski@inteliclinic.com>
 * @date    April, 2017
 * @brief   Brief description
 *
 * Description
 */

#ifndef MACROSCENARIO_H
#define MACROSCENARIO_H

#include "MicroScenario.h"
#include "Scenario.h"
#include "ScenarioTrigger.h"

#include <algorithm>

typedef std::pair<MicroScenario *, int> ScePrioPair;

class MacroScenario : public Scenario {
public:
  MacroScenario(const ncScenarioInitArgs *) {}
  virtual ncUpdateOutput update(ncUnixTimestamp ts, const ncScenarioInput *);
  virtual ~MacroScenario() {}

private:
  struct Cmp {
    bool operator()(const ScePrioPair &lhs, const ScePrioPair &rhs) {
      return lhs.second > rhs.second;
    }
  };

  ncUnixTimestamp _current_moment = 0;
  MicroScenario *_last_active_scenario = nullptr;
  std::vector<ScePrioPair> _scenario_queue;
  std::vector<ScenarioTrigger<const ncScenarioInput *> *>
      _triggers_refresh_list = {};

protected:
  virtual ncUnixTimestamp currentMoment() const { return _current_moment; }

  void addTriggerToRefreshList(ScenarioTrigger<const ncScenarioInput *> *t) {
    _triggers_refresh_list.push_back(t);
  }

  void addScenarioWithPriority(MicroScenario *sc, int priority) {
    _scenario_queue.push_back(std::make_pair(sc, priority));
    std::make_heap(_scenario_queue.begin(), _scenario_queue.end(), Cmp());
  }

  MicroScenario *lastActiveScenario() const { return _last_active_scenario; }
};

#endif /* !MACROSCENARIO_H */
