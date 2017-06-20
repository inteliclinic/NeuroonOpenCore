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

#include "IMicroScenario.h"
#include "Scenario.h"
#include "ScenarioTrigger.h"
#include "TimeTrigger.h"
#include <algorithm>

using ScePrioPair = std::pair<std::shared_ptr<IMicroScenario>, int>;
using MacroScenarioTrigger = ScenarioTrigger<const ncScenarioInput *>;

class MacroScenario : public Scenario {
public:
  MacroScenario(const ncScenarioInitArgs *) {}
  virtual ~MacroScenario() {}

  virtual ncUpdateOutput update(const ncScenarioInput *inp);

  // ---------------- mute

  enum class MuteOperation { SHOULD_MUTE, SHOULD_UNMUTE, MUTED, UNMUTED };

  class {
    bool m_muted;
    bool m_confirmed;
    unsigned int m_time_to_be_muted_sec;
    ncUnixTimestamp m_unmute_ts;

  public:
    void mute(unsigned int t_sec) {
      if (!m_muted) {
        m_muted = true;
        m_confirmed = false;
        m_time_to_be_muted_sec = t_sec;
      }
    }

    void unmute() {
      if (m_muted) {
        m_muted = false;
        m_confirmed = false;
      }
    }

    MuteOperation getMuteOperation(ncUnixTimestamp ts) {
      if (ts >= m_unmute_ts) {
        if (m_muted) {
          m_muted = false;
          m_confirmed = !m_confirmed;
        }
      }
      if (!m_confirmed) {
        if (m_muted) {
          return MuteOperation::SHOULD_MUTE;
        } else {
          return MuteOperation::SHOULD_UNMUTE;
        }
      } else {
        return (m_muted ? MuteOperation::MUTED : MuteOperation::UNMUTED);
      }
    }

    void confirmOperation(ncUnixTimestamp ts) {
      if (!m_confirmed) {
        m_confirmed = true;
        m_unmute_ts = ts + m_time_to_be_muted_sec;
      }
    }

  } m_mute_state;

  virtual void mute(unsigned int time_s) { m_mute_state.mute(time_s); }
  virtual void unmute() { m_mute_state.unmute(); }

private:
  struct Chronological {
    bool operator()(const ScePrioPair &lhs, const ScePrioPair &rhs) {
      return lhs.second > rhs.second;
    }
  };

  ncUnixTimestamp _current_moment = 0;
  IMicroScenario *_recently_used_scenario = nullptr;
  std::vector<ScePrioPair> _scenario_queue;
  std::vector<std::shared_ptr<MacroScenarioTrigger>> _triggers_refresh_list =
      {};

  ScePrioPair
  getHighestPriorityScenario(std::vector<ScePrioPair> scenarios) const;

  // muted
  bool m_muted = false;
  unsigned int m_unmute_time = 0;

protected:
  virtual ncUnixTimestamp currentMoment() const { return _current_moment; }

  void addTriggersToRefreshList(
      std::vector<std::shared_ptr<MacroScenarioTrigger>> ts) {
    for (auto t : ts) {
      _triggers_refresh_list.push_back(t);
    }
  }

  void addScenarioWithPriority(std::shared_ptr<IMicroScenario> sc,
                               int priority) {
    _scenario_queue.push_back(std::make_pair(sc, priority));
    std::make_heap(_scenario_queue.begin(), _scenario_queue.end(),
                   Chronological());
  }

  IMicroScenario *lastActiveScenario() const { return _recently_used_scenario; }

  static std::shared_ptr<MacroScenarioTrigger>
  createMacroScenarioTimeTrigger(int start_time_ms, int end_time_ms,
                                 TimeTriggerMode mode) {
    return std::shared_ptr<MacroScenarioTrigger>(
        TimeTrigger::WithAdapter<const ncScenarioInput *>(
            std::unique_ptr<TimeTrigger>(
                new TimeTrigger(start_time_ms, end_time_ms, mode)),
            [](const ncScenarioInput *inp) {
              return inp->commonInput.currentTime;
            }));
  }

  static std::shared_ptr<MacroScenarioTrigger>
  createMacroScenarioTimeTrigger(std::function<bool(int)> act_fun,
                                 TimeTriggerMode mode) {
    return std::shared_ptr<MacroScenarioTrigger>(
        TimeTrigger::WithAdapter<const ncScenarioInput *>(
            std::unique_ptr<TimeTrigger>(new TimeTrigger(act_fun, mode)),
            [](const ncScenarioInput *inp) {
              return inp->commonInput.currentTime;
            }));
  }

  static std::shared_ptr<MacroScenarioTrigger>
  moveToShared(ScenarioTrigger<const ncScenarioInput *> *t) {
    return std::shared_ptr<MacroScenarioTrigger>(std::move(t));
  }
};

#endif /* !MACROSCENARIO_H */
