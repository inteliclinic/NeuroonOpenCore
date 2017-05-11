#include "LightBoostLightSequenceScenario.h"
#include "ic_low_level_control.h"

MicroScenario::MicroScenarioUpdate
LightBoostLightSequenceScenario::update(bool did_activate,
                                        bool last_instructions) {

  MicroScenarioState state = this->isTriggerActive(kActivationTrigger)
                                 ? MicroScenarioState::WANTS_FOCUS
                                 : MicroScenarioState::IDLE;

  if (last_instructions) {
    // we must deactivate scenario if active
    return {state,
            did_activate ? this->_descendSequence()
                         : std::vector<ncAtomicInstruction>{},
            {}};
  }

  // activation trigger is active and scenarios instruction
  // hasnt been already activated
  if (this->isTriggerActive(kActivationTrigger) && !did_activate) {
    // its time to turn on the lights
    return {state, this->_parametrizedSinusLikeSequence(),
            this->_descendSequence()};
  }
  // if lights are already on
  else if (this->isTriggerActive(kActivationTrigger)) {
    return {state, {}, {}};
  }

  // trigger has just become inactive and scenarios light did activate
  if (this->wasTriggerJustDeactivated(kActivationTrigger) && did_activate) {
    // send turning  off as activation
    return {state, this->_descendSequence(), {}};
  }

  // nothing interesting is happening
  return {state, {}, {}};
}

LightBoostLightSequenceScenario::LightBoostLightSequenceScenario(
    unsigned int ascend_time_ms, unsigned int crest_time_ms,
    unsigned int descend_time_ms, unsigned int trough_time_ms) {

  this->_ascend_time_ms = ascend_time_ms;
  this->_crest_time_ms = crest_time_ms;
  this->_descend_time_ms = descend_time_ms;
  this->_trough_time_ms = trough_time_ms;
  // [TODO] zwykle inicjalizacja zmiennych konfigurujacych sekwencje
}

std::vector<ncAtomicInstruction>
LightBoostLightSequenceScenario::_parametrizedSinusLikeSequence() const {
  // [TODO]
  // ma zwracać wektor instrukcji realizujący światła lightboost
  // zwiększanie intenstywnosci trwa /ascend_time_ms/
  // maksymalna intensywnosc: /crest_time_ms/
  // spadek intensywnosci: /descend_time_ms/
  // wartosc minimalna: /trough_time_ms/
  //
  // zalozenie jest za te instrukcje wywoluja sie cyklicznie do odwolania
  // jezeli tak nie jest to trzeba zmodyfikowac funckje update'u
  // WOJTEK: póki co realizuję tutaj pojedynczą sekwencję "zbocze, wypłaszczenie, zbocze,
  // wypłaszczenie" - trzeba ogarnąć mechanizm, który będzie wywoływał cyklicznie tę funkcję z
  // poziomu mikroscenariusza
  char ble_ramp_up_frame[MASK_INSTRUCTION_LENGTH];
  char ble_ramp_down_frame[MASK_INSTRUCTION_LENGTH];
  size_t len = MASK_INSTRUCTION_LENGTH;
  rgb_led_set_func(ble_ramp_up_frame, &len,
      RGB_LED_SIDE_BOTH,
      FUN_TYPE_RAMP,
      RGB_LED_COLOR_WHITE,
      DEV_MAX_INTENSITY,
      _ascend_time_ms + _crest_time_ms,
      DEV_MIN_PERIOD,
      0); //TODO: potrzebny mechanizm do inkrementowania ID
  rgb_led_set_func(ble_ramp_down_frame, &len,
      RGB_LED_SIDE_BOTH,
      FUN_TYPE_RAMP,
      RGB_LED_COLOR_WHITE,
      0,
      _descend_time_ms + _trough_time_ms,
      DEV_MIN_PERIOD,
      0); //TODO: potrzebny mechanizm do inkrementowania ID

  std::vector<ncAtomicInstruction> instructions;
  instructions.push_back(createAtomicInstruction(0, ble_ramp_up_frame));
  instructions.push_back(createAtomicInstruction(_ascend_time_ms + _crest_time_ms, ble_ramp_down_frame));
  return instructions;
}

std::vector<ncAtomicInstruction>
LightBoostLightSequenceScenario::_descendSequence() const {
  // [TODO]
  // ma zwracać wektor instrukcji realizujący sinusoidalny spadek
  // od wartosci maksymalnej do minimalnej który trwa
  // /descent_time_ms/ milisekund
  // WOJTEK: nie ma takiej opcji póki co - trzeba to obgadać
  return {};
}
