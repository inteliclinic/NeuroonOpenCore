#include "LightBoostLightSequenceScenario.h"

MicroScenario::MicroScenarioUpdate
LightBoostLightSequenceScenario::update(bool did_activate,
                                        bool last_instructions) {
  return {};
}

LightBoostLightSequenceScenario::LightBoostLightSequenceScenario(
    unsigned int ascend_time_ms, unsigned int crest_time_ms,
    unsigned int descend_time_ms, unsigned int trough_time_ms) {

  // [TODO] zwykle inicjalizacja zmiennych konfigurujacych sekwencje
}

std::vector<ncAtomicInstruction>
LightBoostLightSequenceScenario::_descendSequence() const {
  // [TODO]
  // ma zwracać wektor instrukcji realizujący sinusoidalny spadek
  // od wartosci maksymalnej do minimalnej który trwa
  // /descent_time_ms/ milisekund
  return {};
}

// ncUpdateOutput LightBoostLightSequenceScenario::onFinish() {
//   if (this->isFinished()) {
//     return UPDATE_SCENARIO_FINISHED;
//   }
//   // if not already finished we can act as if it was mute command,
//   auto ret = this->onMute();

//   // but finish the scenario afterwards
//   this->finishScenario();
//   return ret;
// }

// ncUpdateOutput LightBoostLightSequenceScenario::onMute() {
//   if (this->isMuted()) {
//     return UPDATE_OK;
//   }
//   // if not already muted provide desactivation instructions
//   this->clearInstructions();
//   this->pushInstructions(this->_descendSequence(),
//   InstructionInsertMode::NOW);
//   return UPDATE_NEW_DATA;
// }

// ncUpdateOutput
// LightBoostLightSequenceScenario::go(const std::set<Key> &triggers) {
//   if (triggers.find(kActivationTrigger) == triggers.end()) {
//     // scenario is not yet active
//     return UPDATE_OK;
//   }

//   // [TODO]
//   // wlasciwa czesc scenariusza.
//   // scenariusz ma realizowac sekwencje swiatel pulsujacego "sinusa".
//   // Kolory i intensywnosc maksymalna i minimalna jest do ustalenia,
//   // albo do wypchniecia jako parametry konfigurujace.
//   //
//   //

//   return UPDATE_OK;
// }
