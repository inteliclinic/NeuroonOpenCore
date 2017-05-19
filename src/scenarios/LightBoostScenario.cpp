#include "LightBoostScenario.h"
#include "LostContactTrigger.h"
#include "TimeTrigger.h"
#include <stdexcept>

LightBoostScenario::LightBoostScenario(const ncScenarioInitArgs *args)
    : MacroScenario(args) {

  this->_intensity = args->lightBoost.intensity;
  this->_length_in_minutes = args->lightBoost.lengthInMinutes;

  // ========== prepare triggers
  // just a lost contact trigger wrapped to accept ncScenarioInput
  auto lost_contact_trigger = LightBoostScenario::moveToShared(

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

  // register triggers
  this->addTriggersToRefreshList(
      {light_sequence_activation_trigger, lost_contact_trigger});

  // ============ scenarios

  // lost contact scenario
  auto lost_contact_light_scenario =
      std::shared_ptr<LightBoostLightSequenceScenario>(
          new LightBoostLightSequenceScenario(500, 9, 500, 500, _length_in_minutes*6000));
  lost_contact_light_scenario->installActivationTrigger(lost_contact_trigger);
  this->addScenarioWithPriority(lost_contact_light_scenario, 2);

  // on contact scenario
  auto on_contact_light_scenario =
      std::shared_ptr<LightBoostLightSequenceScenario>(
          new LightBoostLightSequenceScenario(5000, 24500, 500, 0, _length_in_minutes*6000));

  on_contact_light_scenario->installActivationTrigger(
      light_sequence_activation_trigger);
  this->addScenarioWithPriority(on_contact_light_scenario, 1);
}
