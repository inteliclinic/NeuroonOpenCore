/**
 * @file    MicroScenario.h
 * @Author  Michał Adamczyk <m.adamczyk@inteliclinic.com>
 * @date    April, 2017
 * @brief   Provides a convienient base for implementing IMicroScenario
 *          interface.
 *
 * Description [TODO]
 */

#ifndef MICROSCENARIO_H
#define MICROSCENARIO_H

#include "IMicroScenario.h"
#include "IScenarioTrigger.h"
#include "NeuroonMaskScenariosApi.h"
#include <map>
#include <memory>
#include <queue>
#include <set>

using Key = int;

class MicroScenario : public IMicroScenario {
private:
  ncUnixTimestamp _current_moment = 0;
  bool _will_be_finished = false;
  bool _did_activate = false;
  bool _did_deactivate = false;
  std::shared_ptr<const IScenarioTrigger> _kill_trigger = nullptr;
  MicroScenarioState _state;
  std::vector<ncAtomicInstruction> _activation;
  std::vector<ncAtomicInstruction> _deactivation;
  std::vector<ncAtomicInstruction> _next_deactivation;
  std::map<std::shared_ptr<const IScenarioTrigger>, Key> _trigger_to_key;
  std::set<Key> _previously_active_triggers;
  std::set<Key> _currently_active_triggers;
  std::set<Key> _getCurrentlyActiveTriggers() const;

public:
  virtual ~MicroScenario() {}

  virtual MicroScenarioState getState() const { return _state; }

  virtual void finish() { this->_state = MicroScenarioState::FINISHED; }

  virtual std::vector<ncAtomicInstruction> use();

  virtual std::vector<ncAtomicInstruction> stopUse();

  virtual void refresh(ncUnixTimestamp);

  // ============ triggers

  void
  installKillTrigger(std::shared_ptr<const IScenarioTrigger> kill_trigger) {
    _kill_trigger = kill_trigger;
  }
  void uninstallTrigger(std::shared_ptr<const IScenarioTrigger> trigger) {
    _trigger_to_key.erase(trigger);
  }

protected:
  struct MicroScenarioUpdate {
    MicroScenarioState state_update;
    std::vector<ncAtomicInstruction> activation;
    std::vector<ncAtomicInstruction> deactivation;
  };

  void installTriggerForKey(std::shared_ptr<const IScenarioTrigger> trigger,
                            Key key) {
    _trigger_to_key[trigger] = key;
  }

  bool isTriggerActive(Key) const;
  bool wasTriggerJustActivated(Key) const;
  bool wasTriggerJustDeactivated(Key) const;

  virtual MicroScenarioUpdate
  update(bool did_activate,     // -- last activation set was used
         bool last_instructions // -- it is the last update
         ) = 0;

  ncUnixTimestamp currentMoment() const { return this->_current_moment; }
};

#endif /* !MICROSCENARIO_H */
