/**
 * @file    SleepScenario.cpp
 * @Author  Paweł Kaźmierzewski <p.kazmierzewski@inteliclinic.com>
 * @date    April, 2017
 * @brief   Brief description
 *
 * Description
 */

#include "SleepScenario.h"
#include "ContactTestScenario.h"
#include "HardWakeupMicroScenario.h"
#include "SmartWakeupMicroScenario.h"
#include "NeuroonSunriseMicroScenario.h"
#include "LostContactTrigger.h"
#include "TimeTrigger.h"

SleepScenario::SleepScenario(const ncScenarioInitArgs *args)
    : MacroScenario(args) {

  // === prepare scenario triggers
  auto absolute_wakeup_ts = args->commonSleep.wakeupTimestamp;
  auto smart_wakeup_window_start = args->commonSleep.wakeupTimestamp -
                                   60000 * args->commonSleep.smartWakeupWindow;
  auto artificial_dawn_start = args->commonSleep.artificialSunriseWindow -
                               60000 * args->commonSleep.smartWakeupWindow;

  // absolute wakeup trigger
  auto absolute_wake_trigger = SleepScenario::createMacroScenarioTimeTrigger(
      0, absolute_wakeup_ts, TimeTriggerMode::ABSOLUTE);

  // smart wakeup window trigger
  auto smart_wake_trigger = SleepScenario::createMacroScenarioTimeTrigger(
      smart_wakeup_window_start, absolute_wakeup_ts, TimeTriggerMode::ABSOLUTE);

  // artificial dawn trigger
  auto artificial_dawn_trigger = SleepScenario::createMacroScenarioTimeTrigger(
      artificial_dawn_start, absolute_wakeup_ts, TimeTriggerMode::ABSOLUTE);

  // just a lost contact trigger wrapped to accept ncScenarioInput
  auto lost_contact_trigger = SleepScenario::moveToShared(

      LostContactTrigger::WithAdapter<const ncScenarioInput *>(
          std::unique_ptr<LostContactTrigger>(
              new LostContactTrigger(kContactDetectionTresholdMs)),

          [](const ncScenarioInput *inp) {
            return std::make_pair(
                inp->commonInput.currentTime,
                inp->scenarioSpecific.lightBoost.signalQuality);
          }));

  // register triggers
  this->addTriggersToRefreshList({absolute_wake_trigger, smart_wake_trigger,
                                  artificial_dawn_trigger,
                                  lost_contact_trigger});

  // init and register lost contact scenario
  auto lost_contact_scenario =
      std::shared_ptr<ContactTestScenario>(new ContactTestScenario());
  lost_contact_scenario->installSignalLostTrigger(lost_contact_trigger);
  this->addScenarioWithPriority(lost_contact_scenario, kContactTestPriority);

  // init and register hard wakeup
  auto hard_wakeup_scenario =
    std::shared_ptr<HardWakeupMicroScenario>(
        new HardWakeupMicroScenario(ncWakeUpSequenceIntensity::WAKEUP_ALARM_MEDIUM));
  hard_wakeup_scenario->installActivationTrigger(absolute_wake_trigger);
  this->addScenarioWithPriority(hard_wakeup_scenario, kHardWakeupPriority);

  // init and register smart wakeup
  auto smart_wakeup_scenario =
    std::shared_ptr<SmartWakeupMicroScenario>(new SmartWakeupMicroScenario());
  // [TODO] install triggers
  this->addScenarioWithPriority(smart_wakeup_scenario, kSmartWakeupPriority);

  // init and register sunrise
  auto sunrise_scenario =
    std::shared_ptr<NeuroonSunriseMicroScenario>(new NeuroonSunriseMicroScenario());
  // [TODO] install triggers
  this->addScenarioWithPriority(sunrise_scenario, kSunrisePriority);
}
