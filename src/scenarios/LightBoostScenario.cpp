#include "LightBoostScenario.h"
#include "LostContactTrigger.h"
#include "TimeTrigger.h"
#include <stdexcept>

LightBoostScenario::LightBoostScenario(const ncScenarioInitArgs *args)
    : MacroScenario(args) {

  this->_intensity = args->lightBoost.intensity;
  this->_length_in_minutes = args->lightBoost.lengthInMinutes;

  // just a lost contact trigger wrapped to accept ncScenarioInput
  auto lost_contact_trigger = this->moveToShared(

      LostContactTrigger::WithAdapter<const ncScenarioInput *>(
          std::unique_ptr<LostContactTrigger>(
              new LostContactTrigger(kContactDetectionTresholdMs)),

          [](const ncScenarioInput *inp) {
            return std::make_pair(
                inp->commonInput.currentTime,
                inp->scenarioSpecific.lightBoost.signalQuality);
          }));

  // time trigger that will finish scenarios
  auto treshold = kContactDetectionTresholdMs;
  auto light_sequence_activation_trigger = this->moveToShared(

      new StatefulTrigger<ncUnixTimestamp, const ncScenarioInput *>(
          0, [treshold](ncUnixTimestamp &s, const ncScenarioInput *inp) {
            if (s == 0) {
              s = inp->commonInput.currentTime;
            }
            return inp->commonInput.currentTime - s >= treshold;
          }));

  this->_lost_contact_light_scenario.installActivationTrigger(
      lost_contact_trigger);
  this->addScenarioWithPriority(&this->_lost_contact_light_scenario, 2);

  // this is trigger that accept ncScenarioInput and will activate after
  // /treshold/ ms
  this->_on_contact_light_scenario.installActivationTrigger(
      light_sequence_activation_trigger);
  this->addScenarioWithPriority(&this->_on_contact_light_scenario, 1);

  this->addTriggersToRefreshList(
      {light_sequence_activation_trigger, lost_contact_trigger});
}
