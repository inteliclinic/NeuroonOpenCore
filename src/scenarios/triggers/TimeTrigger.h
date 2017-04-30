/**
 * @file    TimeTrigger.h
 * @Author  Michał Adamczyk <m.adamczyk@inteliclinic.com>
 * @date    April, 2017
 * @brief   Simple trigger that will be active when function on time
 *          returns true.
 *
 */

#ifndef TIMETRIGGER_H
#define TIMETRIGGER_H

#include "NeuroonApiCommons.h"
#include "ScenarioTrigger.h"
#include <functional>

enum class TimeTriggerMode { PASSED, ABSOLUTE };

class TimeTrigger : public StatefulTrigger<ncUnixTimestamp, ncUnixTimestamp> {

public:
  TimeTrigger(int start_time_ms, int end_time_ms, TimeTriggerMode  mode)
      : TimeTrigger(
            [start_time_ms, end_time_ms](int passed_ms) {
              return passed_ms >= start_time_ms && passed_ms <= end_time_ms;
            },
            mode) {}

  TimeTrigger(std::function<bool(int)> time_activation, TimeTriggerMode  mode)
      : StatefulTrigger(
            0, [time_activation, mode](ncUnixTimestamp &s,
                                       ncUnixTimestamp current_time) {
              if (s == 0)
                s = current_time;
              auto time_arg =
                  mode == TimeTriggerMode::PASSED ? current_time - s : current_time;
              return time_activation(time_arg);
            }) {}
};

#endif /* !TIMETRIGGER_H */
