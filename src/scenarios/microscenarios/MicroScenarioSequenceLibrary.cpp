/**
 * @file    MicroScenarioSequenceLibrary.cpp
 * @Author  Wojciech Weclewski <w.weclewski@inteliclinic.com>
 * @date    May, 2017
 * @brief   Brief description
 *
 * Description
 */

#include "MicroScenarioSequenceLibrary.h"
#include <cstring>
#include <string>
#include "ic_low_level_control.h"

namespace sequence{

  enum{
    RED_LED = 0,
    GREEN_LED,
    BLUE_LED,
    NUM_OF_LEDS
  };

  const unsigned int NUM_OF_ART_DAWN_STEPS = 40;

  const uint8_t artificial_dawn[NUM_OF_ART_DAWN_STEPS][NUM_OF_LEDS] =
  {
    {0, 0, 0},
    {1, 0, 0},
    {4, 1, 0},
    {8, 1, 0},
    {13, 2, 0},
    {19, 3, 0},
    {19, 3, 0},
    {18, 3, 0},
    {18, 4, 0},
    {18, 4, 0},
    {17, 4, 0},
    {17, 5, 0},
    {17, 5, 0},
    {16, 5, 0},
    {19, 10, 1},
    {19, 11, 1},
    {18, 11, 1},
    {18, 12, 1},
    {17, 12, 1},
    {17, 13, 1},
    {16, 13, 1},
    {16, 20, 2},
    {15, 21, 2},
    {14, 22, 2},
    {13, 23, 2},
    {13, 38, 3},
    {12, 39, 3},
    {11, 40, 3},
    {11, 55, 4},
    {11, 62, 5},
    {11, 62, 10},
    {11, 62, 15},
    {11, 62, 20},
    {11, 62, 25},
    {11, 62, 30},
    {11, 62, 35},
    {11, 62, 40},
    {11, 62, 47},
    {11, 62, 55},
    {11, 62, 62}
  };

  static ncAtomicInstruction createAtomicInstruction(unsigned int time, char (&data)[MASK_INSTRUCTION_LENGTH]){
    ncAtomicInstruction atomic_instruction;
    atomic_instruction.time = time;
    std::memcpy(atomic_instruction.data, data, sizeof(data));
    return atomic_instruction;
  }

  std::vector<ncAtomicInstruction> descendSequence(unsigned int descend_time_ms){
    // [TODO]
    // ma zwracać wektor instrukcji realizujący sinusoidalny spadek
    // od wartosci maksymalnej do minimalnej który trwa
    // /descent_time_ms/ milisekund
    // WOJTEK: nie ma takiej opcji póki co - trzeba to obgadać
    char ble_ramp_down_frame[MASK_INSTRUCTION_LENGTH];
    size_t len = MASK_INSTRUCTION_LENGTH;
    rgb_led_set_func(ble_ramp_down_frame, &len,
        RGB_LED_SIDE_BOTH,
        FUN_TYPE_RAMP,
        RGB_LED_COLOR_WHITE,
        0,
        descend_time_ms,
        DEV_MIN_PERIOD,
        0); //TODO: potrzebny mechanizm do inkrementowania ID
    std::vector<ncAtomicInstruction> instructions;
    instructions.push_back(createAtomicInstruction(0, ble_ramp_down_frame));
    return instructions;
  }

  std::vector<ncAtomicInstruction> parametrizedSinusLikeSequence(unsigned int ascend_time_ms, unsigned int crest_time_ms, unsigned int descend_time_ms, unsigned int trough_time_ms, unsigned int duration_ms){
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
        DEV_INF_DURATION,
        DEV_MIN_PERIOD,
        0); //TODO: potrzebny mechanizm do inkrementowania ID
    rgb_led_set_func(ble_ramp_down_frame, &len,
        RGB_LED_SIDE_BOTH,
        FUN_TYPE_RAMP,
        RGB_LED_COLOR_WHITE,
        0,
        DEV_INF_DURATION,
        DEV_MIN_PERIOD,
        0); //TODO: potrzebny mechanizm do inkrementowania ID

    std::vector<ncAtomicInstruction> instructions;
    for(unsigned int i = 0, atom_seq_time_ms = ascend_time_ms + crest_time_ms + descend_time_ms + trough_time_ms, end = duration_ms/atom_seq_time_ms; i < end; i++){
      instructions.push_back(createAtomicInstruction(i*atom_seq_time_ms, ble_ramp_up_frame));
      instructions.push_back(createAtomicInstruction(i*atom_seq_time_ms + ascend_time_ms + crest_time_ms, ble_ramp_down_frame));
    }

    return instructions;
  }

  static inline void setAllRGBForNUC(uint8_t &device){
    DEV_SET(device, DEV_RIGHT_RED_LED |
                    DEV_RIGHT_GREEN_LED |
                    DEV_RIGHT_BLUE_LED |
                    DEV_LEFT_RED_LED |
                    DEV_LEFT_GREEN_LED |
                    DEV_LEFT_BLUE_LED);
  }

  static void getSelectedADIntensity(uint8_t (&intensity)[7], unsigned int step){
    for(unsigned int i = 0; i < NUM_OF_LEDS; i++){
      intensity[i] = intensity[i+NUM_OF_LEDS] = artificial_dawn[step][i];
    }
  }

  std::vector<ncAtomicInstruction> artificialDawnSequence(unsigned int duration_ms){
    unsigned int time_step = duration_ms/NUM_OF_ART_DAWN_STEPS;

    uint8_t device;
    setAllRGBForNUC(device);

    char ble_ramp_up_frame[MASK_INSTRUCTION_LENGTH];
    size_t len = MASK_INSTRUCTION_LENGTH;

    uint8_t intensity[7] = { 0 }; //Because there are 7 LTC devices without power LEDs

    std::vector<ncAtomicInstruction> instructions;
    for(unsigned int i = 0; i<NUM_OF_ART_DAWN_STEPS; i++){
      getSelectedADIntensity(intensity, i);
      device_set_func(ble_ramp_up_frame, &len,
          device,
          FUN_TYPE_RAMP,
          intensity,
          DEV_INF_DURATION,
          DEV_MIN_PERIOD,
          0); //TODO: potrzebny mechanizm do inkrementowania ID
      instructions.push_back(createAtomicInstruction(i*time_step, ble_ramp_up_frame));
    }
    return instructions;
  }

  static inline std::vector<ncAtomicInstruction> getSoftAlarm(){
    char ble_rgb_frame[MASK_INSTRUCTION_LENGTH];
    size_t len = MASK_INSTRUCTION_LENGTH;
    rgb_led_set_func(ble_rgb_frame, &len,
        RGB_LED_SIDE_BOTH,
        FUN_TYPE_SIN_WAVE,
        RGB_LED_COLOR_WHITE,
        DEV_MAX_INTENSITY,
        DEV_INF_DURATION,
        20, //2 second in hundreds of ms
        0); //TODO: potrzebny mechanizm do inkrementowania ID

    std::vector<ncAtomicInstruction> instructions;
    instructions.push_back(createAtomicInstruction(0, ble_rgb_frame));
    return instructions;
  }

  static inline std::vector<ncAtomicInstruction> getMediumAlarm(){
    char ble_rgb_frame[MASK_INSTRUCTION_LENGTH];
    char ble_vib_frame[MASK_INSTRUCTION_LENGTH];
    size_t len = MASK_INSTRUCTION_LENGTH;
    rgb_led_set_func(ble_rgb_frame, &len,
        RGB_LED_SIDE_BOTH,
        FUN_TYPE_SIN_WAVE,
        RGB_LED_COLOR_WHITE,
        DEV_MAX_INTENSITY,
        DEV_INF_DURATION,
        10, //1 second in hundreds of ms
        0); //TODO: potrzebny mechanizm do inkrementowania ID
    vibrator_set_func(ble_vib_frame, &len,
        FUN_TYPE_SIN_WAVE,
        50,
        DEV_INF_DURATION,
        20, //2 second in hundreds of ms
        0); //TODO: potrzebny mechanizm do inkrementowania ID

    std::vector<ncAtomicInstruction> instructions;
    instructions.push_back(createAtomicInstruction(0, ble_rgb_frame));
    instructions.push_back(createAtomicInstruction(0, ble_vib_frame));
    return instructions;
  }

  static inline std::vector<ncAtomicInstruction> getHardAlarm(){
    char ble_rgb_frame[MASK_INSTRUCTION_LENGTH];
    char ble_vib_frame[MASK_INSTRUCTION_LENGTH];
    char ble_pwr_frame[MASK_INSTRUCTION_LENGTH];
    size_t len = MASK_INSTRUCTION_LENGTH;
    rgb_led_set_func(ble_rgb_frame, &len,
        RGB_LED_SIDE_BOTH,
        FUN_TYPE_SIN_WAVE,
        RGB_LED_COLOR_WHITE,
        DEV_MAX_INTENSITY,
        DEV_INF_DURATION,
        10, //1 second in hundreds of ms
        0); //TODO: potrzebny mechanizm do inkrementowania ID
    vibrator_set_func(ble_vib_frame, &len,
        FUN_TYPE_SQUARE,
        DEV_MAX_INTENSITY,
        DEV_INF_DURATION,
        10, //1 second in hundreds of ms
        0); //TODO: potrzebny mechanizm do inkrementowania ID
    pwr_led_set_func(ble_pwr_frame, &len,
        FUN_TYPE_ON,
        DEV_INF_DURATION,
        5, //0.5 second in hundreds of ms
        0); //TODO: potrzebny mechanizm do inkrementowania ID

    std::vector<ncAtomicInstruction> instructions;
    instructions.push_back(createAtomicInstruction(0, ble_rgb_frame));
    instructions.push_back(createAtomicInstruction(0, ble_vib_frame));
    instructions.push_back(createAtomicInstruction(0, ble_pwr_frame));
    return instructions;
  }

  std::vector<ncAtomicInstruction> wakeUpSequence(wakeUpSequenceIntensity mode){
    switch(mode){
      case WU_ALARM_SOFT:    return getSoftAlarm();
      case WU_ALARM_MEDIUM:  return getMediumAlarm();
      case WU_ALARM_HARD:    return getHardAlarm();
      default:            return getSoftAlarm();
    }
  }

  std::vector<ncAtomicInstruction> lucidDreamSequence(){
    char ble_pwr_led_frame[MASK_INSTRUCTION_LENGTH];
    char ble_red_blink_frame[MASK_INSTRUCTION_LENGTH];
    char ble_blue_blink_frame[MASK_INSTRUCTION_LENGTH];
    size_t len = MASK_INSTRUCTION_LENGTH;
    pwr_led_set_func(ble_pwr_led_frame, &len,
        FUN_TYPE_ON,
        50, //5 seconds in hundreds of ms
        10, //1 second in hundreds of ms
        0); //TODO: potrzebny mechanizm do inkrementowania ID
    rgb_led_set_func(ble_red_blink_frame, &len,
        RGB_LED_SIDE_BOTH,
        FUN_TYPE_BLINK,
        RGB_LED_COLOR_RED,
        DEV_MAX_INTENSITY,
        DEV_INF_DURATION,
        DEV_MIN_PERIOD,
        0); //TODO: potrzebny mechanizm do inkrementowania ID
    rgb_led_set_func(ble_blue_blink_frame, &len,
        RGB_LED_SIDE_BOTH,
        FUN_TYPE_BLINK,
        RGB_LED_COLOR_BLUE,
        DEV_MAX_INTENSITY,
        DEV_INF_DURATION,
        DEV_MIN_PERIOD,
        0); //TODO: potrzebny mechanizm do inkrementowania ID

    std::vector<ncAtomicInstruction> instructions;
    instructions.push_back(createAtomicInstruction(0, ble_pwr_led_frame)); //Hardcoded sequence!!!
    instructions.push_back(createAtomicInstruction(5000, ble_blue_blink_frame));
    instructions.push_back(createAtomicInstruction(6000, ble_red_blink_frame));
    instructions.push_back(createAtomicInstruction(7000, ble_blue_blink_frame));
    instructions.push_back(createAtomicInstruction(8000, ble_blue_blink_frame));
    instructions.push_back(createAtomicInstruction(9000, ble_red_blink_frame));

    return instructions;
  }

  std::vector<ncAtomicInstruction> circadianShiftSequence(){
    char ble_rgb_frame[MASK_INSTRUCTION_LENGTH];
    size_t len = MASK_INSTRUCTION_LENGTH;
    rgb_led_set_func(ble_rgb_frame, &len,
        RGB_LED_SIDE_BOTH,
        FUN_TYPE_SQUARE,
        RGB_LED_COLOR_RED,
        DEV_MAX_INTENSITY,
        DEV_INF_DURATION,
        10, //1 second in hundreds of ms
        0); //TODO: potrzebny mechanizm do inkrementowania ID

    std::vector<ncAtomicInstruction> instructions;
    instructions.push_back(createAtomicInstruction(0, ble_rgb_frame));
  }

  std::vector<ncAtomicInstruction> noContactSequence(){
    char ble_green_sin_frame[MASK_INSTRUCTION_LENGTH];
    size_t len = MASK_INSTRUCTION_LENGTH;
    rgb_led_set_func(ble_green_sin_frame, &len,
        RGB_LED_SIDE_BOTH,
        FUN_TYPE_SIN_WAVE,
        RGB_LED_COLOR_GREEN,
        10,
        DEV_INF_DURATION,
        20, //1 second in hundreds of ms
        0); //TODO: potrzebny mechanizm do inkrementowania ID

    std::vector<ncAtomicInstruction> instructions;
    instructions.push_back(createAtomicInstruction(0, ble_green_sin_frame));

    return instructions;
  }

  static inline void setAllDevicesForNUC(uint8_t &device){
    DEV_SET(device, DEV_RIGHT_RED_LED |
                    DEV_RIGHT_GREEN_LED |
                    DEV_RIGHT_BLUE_LED |
                    DEV_LEFT_RED_LED |
                    DEV_LEFT_GREEN_LED |
                    DEV_LEFT_BLUE_LED |
                    DEV_VIBRATOR |
                    DEV_POWER_LED);
  }

  std::vector<ncAtomicInstruction> killSequence(){
    char ble_ramp_down_frame[MASK_INSTRUCTION_LENGTH];
    char ble_turn_off_frame[MASK_INSTRUCTION_LENGTH];
    size_t len = MASK_INSTRUCTION_LENGTH;

    uint8_t device;
    setAllDevicesForNUC(device);

    device_set_func(ble_ramp_down_frame, &len,
        device,
        FUN_TYPE_RAMP,
        0,
        DEV_INF_DURATION,
        DEV_MIN_PERIOD,
        0); //TODO: potrzebny mechanizm do inkrementowania ID
    device_set_func(ble_turn_off_frame, &len,
        device,
        FUN_TYPE_OFF,
        0,
        0,
        0,
        0); //TODO: potrzebny mechanizm do inkrementowania ID

    std::vector<ncAtomicInstruction> instructions;
    instructions.push_back(createAtomicInstruction(0, ble_ramp_down_frame));
    instructions.push_back(createAtomicInstruction(2000, ble_turn_off_frame));

    return instructions;
  }
}

namespace msequence{

  std::vector<ncAtomicInstruction> lucidDreamMocSequence(){
    char ble_frame_1[MASK_INSTRUCTION_LENGTH];
    char ble_frame_2[MASK_INSTRUCTION_LENGTH];
    size_t len = MASK_INSTRUCTION_LENGTH;
    rgb_led_set_func(ble_frame_1, &len,
        RGB_LED_SIDE_BOTH,
        FUN_TYPE_ON,
        RGB_LED_COLOR_GREEN,
        DEV_MAX_INTENSITY,
        DEV_INF_DURATION,
        DEV_MIN_PERIOD,
        0); //TODO: potrzebny mechanizm do inkrementowania ID
    rgb_led_set_func(ble_frame_2, &len,
        RGB_LED_SIDE_BOTH,
        FUN_TYPE_OFF,
        RGB_LED_COLOR_GREEN,
        0,
        DEV_INF_DURATION,
        DEV_MIN_PERIOD,
        0); //TODO: potrzebny mechanizm do inkrementowania ID

    std::vector<ncAtomicInstruction> instructions;
    for(unsigned int i = 0; i < 300000/2000; i++){
      instructions.push_back(sequence::createAtomicInstruction(i*2000, ble_frame_1));
      instructions.push_back(sequence::createAtomicInstruction(i*2000+1000, ble_frame_2));
    }

    return instructions;
  }

  std::vector<ncAtomicInstruction> lightBoostMocSequence(){
    char ble_frame_1[MASK_INSTRUCTION_LENGTH];
    char ble_frame_2[MASK_INSTRUCTION_LENGTH];
    size_t len = MASK_INSTRUCTION_LENGTH;
    rgb_led_set_func(ble_frame_1, &len,
        RGB_LED_SIDE_BOTH,
        FUN_TYPE_ON,
        RGB_LED_COLOR_BLUE,
        DEV_MAX_INTENSITY,
        DEV_INF_DURATION,
        DEV_MIN_PERIOD,
        0); //TODO: potrzebny mechanizm do inkrementowania ID
    rgb_led_set_func(ble_frame_2, &len,
        RGB_LED_SIDE_BOTH,
        FUN_TYPE_OFF,
        RGB_LED_COLOR_BLUE,
        0,
        DEV_INF_DURATION,
        DEV_MIN_PERIOD,
        0); //TODO: potrzebny mechanizm do inkrementowania ID

    std::vector<ncAtomicInstruction> instructions;
    for(unsigned int i = 0; i < 300000/2000; i++){
      instructions.push_back(sequence::createAtomicInstruction(i*2000, ble_frame_1));
      instructions.push_back(sequence::createAtomicInstruction(i*2000+1000, ble_frame_2));
    }

    return instructions;
  }

  std::vector<ncAtomicInstruction> sleepMocSequence(){
    char ble_frame_1[MASK_INSTRUCTION_LENGTH];
    char ble_frame_2[MASK_INSTRUCTION_LENGTH];
    size_t len = MASK_INSTRUCTION_LENGTH;
    rgb_led_set_func(ble_frame_1, &len,
        RGB_LED_SIDE_BOTH,
        FUN_TYPE_ON,
        RGB_LED_COLOR_WHITE,
        DEV_MAX_INTENSITY,
        DEV_INF_DURATION,
        DEV_MIN_PERIOD,
        0); //TODO: potrzebny mechanizm do inkrementowania ID
    rgb_led_set_func(ble_frame_2, &len,
        RGB_LED_SIDE_BOTH,
        FUN_TYPE_OFF,
        RGB_LED_COLOR_WHITE,
        0,
        DEV_INF_DURATION,
        DEV_MIN_PERIOD,
        0); //TODO: potrzebny mechanizm do inkrementowania ID

    std::vector<ncAtomicInstruction> instructions;
    for(unsigned int i = 0; i < 300000/2000; i++){
      instructions.push_back(sequence::createAtomicInstruction(i*2000, ble_frame_1));
      instructions.push_back(sequence::createAtomicInstruction(i*2000+1000, ble_frame_2));
    }

    return instructions;
  }

  std::vector<ncAtomicInstruction> powerNapMocSequence(){
    char ble_frame_1[MASK_INSTRUCTION_LENGTH];
    char ble_frame_2[MASK_INSTRUCTION_LENGTH];
    size_t len = MASK_INSTRUCTION_LENGTH;
    rgb_led_set_func(ble_frame_1, &len,
        RGB_LED_SIDE_BOTH,
        FUN_TYPE_ON,
        RGB_LED_COLOR_RED,
        DEV_MAX_INTENSITY,
        DEV_INF_DURATION,
        DEV_MIN_PERIOD,
        0); //TODO: potrzebny mechanizm do inkrementowania ID
    rgb_led_set_func(ble_frame_2, &len,
        RGB_LED_SIDE_BOTH,
        FUN_TYPE_ON,
        RGB_LED_COLOR_GREEN,
        DEV_MAX_INTENSITY,
        DEV_INF_DURATION,
        DEV_MIN_PERIOD,
        0); //TODO: potrzebny mechanizm do inkrementowania ID

    std::vector<ncAtomicInstruction> instructions;
    for(unsigned int i = 0; i < 300000/2000; i++){
      instructions.push_back(sequence::createAtomicInstruction(i*2000, ble_frame_1));
      instructions.push_back(sequence::createAtomicInstruction(i*2000+1000, ble_frame_2));
    }

    return instructions;
  }

  std::vector<ncAtomicInstruction> circadianRythmMocSequence(){
    char ble_frame_1[MASK_INSTRUCTION_LENGTH];
    char ble_frame_2[MASK_INSTRUCTION_LENGTH];
    size_t len = MASK_INSTRUCTION_LENGTH;
    rgb_led_set_func(ble_frame_1, &len,
        RGB_LED_SIDE_BOTH,
        FUN_TYPE_ON,
        RGB_LED_COLOR_RED,
        DEV_MAX_INTENSITY,
        DEV_INF_DURATION,
        DEV_MIN_PERIOD,
        0); //TODO: potrzebny mechanizm do inkrementowania ID
    rgb_led_set_func(ble_frame_2, &len,
        RGB_LED_SIDE_BOTH,
        FUN_TYPE_ON,
        RGB_LED_COLOR_BLUE,
        DEV_MAX_INTENSITY,
        DEV_INF_DURATION,
        DEV_MIN_PERIOD,
        0); //TODO: potrzebny mechanizm do inkrementowania ID

    std::vector<ncAtomicInstruction> instructions;
    for(unsigned int i = 0; i < 300000/2000; i++){
      instructions.push_back(sequence::createAtomicInstruction(i*2000, ble_frame_1));
      instructions.push_back(sequence::createAtomicInstruction(i*2000+1000, ble_frame_2));
    }

    return instructions;
  }
}
