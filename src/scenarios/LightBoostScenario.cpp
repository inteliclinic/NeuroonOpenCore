#include "LightBoostScenario.h"
#include "LostContactTrigger.h"
#include <stdexcept>

LightBoostScenario::LightBoostScenario(const ncScenarioInitArgs *args)
    : MacroScenario(args) {

  this->_intensity = args->lightBoost.intensity;
  this->_length_in_minutes = args->lightBoost.lengthInMinutes;

  // just a lost contact trigger wrapped to accept ncScenarioInput
  auto lost_contact_trigger =
      LostContactTrigger::WithAdapter<const ncScenarioInput *>(
          std::unique_ptr<LostContactTrigger>(
              new LostContactTrigger(kContactDetectionTresholdMs)),
          [](const ncScenarioInput *inp) {
            return std::make_pair(inp->commonInput.currentTime,
                                  inp->scenarioSpecific.lightBoost.signalQuality);
          });
  this->_lost_contact_light_scenario.installActivationTrigger(
      lost_contact_trigger);
  this->addScenarioWithPriority(&this->_lost_contact_light_scenario,2);
  this->addTriggerToRefreshList(lost_contact_trigger);
  this->_triggers_to_be_deleted.push_back(lost_contact_trigger);

  auto treshold = kContactDetectionTresholdMs;
  // this is trigger that accept ncScenarioInput and will activate after
  // /treshold/ ms
  auto light_sequence_activation_trigger =
      new StatefulTrigger<ncUnixTimestamp, const ncScenarioInput *>(
          0, [treshold](ncUnixTimestamp &s, const ncScenarioInput *inp) {
            if (s == 0) {
              s = inp->commonInput.currentTime;
            }
            return inp->commonInput.currentTime - s >= treshold;
          });
  this->addTriggerToRefreshList(light_sequence_activation_trigger);
  this->_triggers_to_be_deleted.push_back(light_sequence_activation_trigger);
  this->_on_contact_light_scenario.installActivationTrigger(
      light_sequence_activation_trigger);
  this->addScenarioWithPriority(&this->_on_contact_light_scenario,1);
}

ncUpdateOutput LightBoostScenario::update(const ncScenarioInput *update_args) {
  // manually setup microscenarios finishing and go with update procedure
  auto ts = update_args->commonInput.currentTime;
  if(this->_start_ts == 0){
    this->_start_ts = ts;
  }
  else if(static_cast<unsigned int>(ts - this->_start_ts) > 60 * 1000 * this->_length_in_minutes){
    this->_lost_contact_light_scenario.refresh(ts, MicroScenarioControl::FINISH);
    this->_on_contact_light_scenario.refresh(ts, MicroScenarioControl::FINISH);
  }
  return MacroScenario::update(update_args);
}
