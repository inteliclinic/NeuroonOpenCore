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

class TimeTrigger
    : public StatefulTrigger<ncUnixTimestamp, ncUnixTimestamp> {

  enum class TimeMode{
    PASSED,
    ABSOLUTE
  };

  TimeTrigger(std::function<bool(int)> time_activation, TimeMode mode)
    : StatefulTrigger(0, [time_activation, mode](ncUnixTimestamp &s,
                                                 ncUnixTimestamp current_time) {
                        if (s == 0)
                          s = current_time;
                        auto time_arg = mode == TimeMode::PASSED ? current_time - s : current_time;
                        return time_activation(time_arg);
                      }) {}
public:

  static TimeTrigger Passed(int start_time_ms, int end_time_ms ){
    return TimeTrigger([start_time_ms, end_time_ms](int passed_ms) {
        return passed_ms >= start_time_ms && passed_ms <= end_time_ms;
      }, TimeMode::PASSED);
  }

  static TimeTrigger Passed(std::function<bool(int)> time_activation){
    return TimeTrigger(time_activation, TimeMode::PASSED);
  }

  static TimeTrigger Absolute(ncUnixTimestamp start_time_ms, ncUnixTimestamp end_time_ms){
    return TimeTrigger([start_time_ms, end_time_ms](int passed_ms) {
        return passed_ms >= start_time_ms && passed_ms <= end_time_ms;
      }, TimeMode::ABSOLUTE);
  }

  static TimeTrigger Absolute(std::function<bool(int)> time_activation){
    return TimeTrigger(time_activation, TimeMode::ABSOLUTE);
  }

};

#endif /* !TIMETRIGGER_H */
