/**
 * @file    ScenariosFrontEnd.cpp
 * @Author  Paweł Kaźmierzewski <p.kazmierzewski@inteliclinic.com>
 * @date    December, 2016
 * @brief   Brief description
 *
 * Description
 */

#include <queue>
#include <cstring>
#include <iostream>
#include "NeuroonMaskScenariosApi.h"
#include "ic_low_level_control.h"
#include "GenericScenarioTemplates.h"

enum ScenarioStates{
  INITIAL_STATE = 0x01,
  FIRST_REM_STATE,
  SECOND_REM_STATE
};
/////////////////////////////////Michaltmp//////////////////////////////////////////////////////////
typedef struct{
  unsigned long timestamp;
  bool rem;
}ncMichalScenarioInput;
///////////////////////////////////////////////////////////////////////////////////////////////////
union ScenarioInput{
  ncLucidDreamScenarioInput lucidDream;
  ncWakeUpScenarioInput wakeUp;
  ncMichalScenarioInput michal;
  ncPowernapScenarioInput powerNap;
  ncLightBoostScenarioInput lightBoost;
  ncCircadianRhythmAdjustmentScenarioInput circadianRyth;
};

struct BaseScenario{
  std::queue<ncAtomicInstruction> dataFiFo;
  unsigned long endTime;
  ScenarioStates state;
};

struct LucidDreamScenario:BaseScenario{
  ncLucidPulsesIntensity startingIntensity;
  ncLucidRemStabilityTreshold remStabilityTreshold;

  unsigned long remCounter;
  unsigned long remStartTimestamp;
  bool remDetected;
  bool remCounted;

  bool lucidLoaded;

  LucidDreamScenario() :
    remCounter(0),
    remStartTimestamp(0),
    remDetected(false),
    remCounted(false),
    lucidLoaded(false) {}
};

struct WakeUpScenario:BaseScenario{
};

struct PowernapScenario:BaseScenario{
};

struct CircadianRhythmAdjustmentScenario:BaseScenario{
};

struct LightBoostScenario:BaseScenario{
  unsigned int lenthInSeconds;
  unsigned char intensity;
  LightBoostScenario() :
    lenthInSeconds(0),
    intensity(0) {}
};

static std::queue<ncAtomicInstruction> loadDefaultScenario(void);
static ncAtomicInstruction getNextMaskInstruction(BaseScenario *scenario);
static bool availableMaskInstruction(ncLucidDreamScenario *scenario);

ncLucidDreamScenario * ncLucidInitScenario(ncLucidDreamScenarioArgs initArgs){
  auto *_scenario = new LucidDreamScenario;

  _scenario->remStabilityTreshold = initArgs.remStabilityTreshold;
  _scenario->startingIntensity = initArgs.startingIntensity;
  _scenario->dataFiFo = loadDefaultScenario();

  return _scenario;
}

ncLightBoostScenario *ncLbInitScenario(ncLightBoostInitScenarioArgs initArgs){
  auto _scenario = new ncLightBoostScenario;
  _scenario->lenthInSeconds = initArgs.lenthInMinutes * 60;
  _scenario->intensity = initArgs.intensity == ncLightIntensityLevel::REGULAR ? 45 : 63; // this value has to be updated somehow
  ncAtomicInstruction _instruction;
  size_t _len = 20;

  rgb_led_set_func(_instruction.data, &_len, RGB_LED_SIDE_BOTH, FUN_TYPE_ON, RGB_LED_COLOR_WHITE,
      _scenario->intensity, 0, 0, 1);
  _instruction.time = 0;
  _scenario->dataFiFo.push(_instruction);

  rgb_led_set_func(_instruction.data, &_len, RGB_LED_SIDE_BOTH, FUN_TYPE_OFF, RGB_LED_COLOR_WHITE,
      0, 0, 0, 1);
  _instruction.time = _scenario->lenthInSeconds*1000;
  _scenario->dataFiFo.push(_instruction);

  return _scenario;
}

void ncLbDestroyScenario(ncLightBoostScenario *_scenario){
  delete _scenario;
}

ncAtomicInstruction ncLbGetNextMaskInstruction(ncLightBoostScenario *scenario){
  return getNextMaskInstruction(scenario);
}

ncMaskInstructionList ncLbGetMaskInstructions(ncLightBoostScenario *scenario,
                         const ncLightBoostScenarioInput *updateArgs){
  ncMaskInstructionList retVal = {0, NULL};
  return retVal;
}

struct LucidDreamSequenceInput{
  unsigned long length;
  unsigned long numberOfActions;
  unsigned long actionDuration;
  unsigned long actionPeriod;

  LucidDreamSequenceInput(
      unsigned long length, unsigned long numberOfActions,
      unsigned long actionDuration, unsigned long actionPeriod){
    this->length = length;
    this->numberOfActions = numberOfActions;
    this->actionDuration = actionDuration;
    this->actionPeriod = actionPeriod;
  }
};

static void lucidDreamSequence(ncLucidDreamScenario *scenario, LucidDreamSequenceInput sequence,
    unsigned long timestamp){
  ncAtomicInstruction instruction;
  uint8_t intensities[7];
  std::memset(intensities, 63, sizeof(intensities));
  size_t len;
  for(unsigned long i=0; i<sequence.numberOfActions; ++i){
    device_set_func(instruction.data, &len, DEV_POWER_LED|DEV_RIGHT_RED_LED|DEV_LEFT_RED_LED,
        FUN_TYPE_BLINK, intensities, sequence.actionDuration/100, sequence.actionPeriod/100, 0);
    instruction.time = timestamp;
    scenario->dataFiFo.push(instruction);
    timestamp += sequence.numberOfActions>1 ? sequence.length/(sequence.numberOfActions - 1) : 0;
  }
}

bool ncLucidUpdate(ncLucidDreamScenario *scenario, const ncLucidDreamScenarioInput *updateArgs){
  #define rem_duration (updateArgs->timestamp - scenario->remStartTimestamp)

  if(scenario->remDetected)
    if(rem_duration >= 180000
        && !scenario->remCounted){//TODO: 180000 needs to be a parameter
      scenario->remCounter++;
      scenario->remCounted = true;
    }

  if(updateArgs->currentSleep_stage == REM){
    if(!scenario->remDetected){
      scenario->remStartTimestamp = updateArgs->timestamp;
      scenario->remDetected = true;
      scenario->remCounted = false;
    }
    else{
    }
  }
  else{
    if(scenario->remDetected){
      scenario->remDetected = false;
      scenario->remCounted = false;
    }
    scenario->lucidLoaded = false;
  }


  switch(scenario->remCounter){
    case 0:
    case 1:
    case 2:
      break;
    case 3:
      if(rem_duration >= 300000){//TODO: 300000 needs to ba a parameter
        if(!scenario->lucidLoaded){
          lucidDreamSequence(scenario, LucidDreamSequenceInput(240000, 5, 5000, 1000),
              updateArgs->timestamp);
          scenario->lucidLoaded = true;
          return true;
        }
      }
      break;
    default:
      if(rem_duration >= 600000){//TODO: 600000 needs to ba a parameter
        if(!scenario->lucidLoaded){
          lucidDreamSequence(scenario, LucidDreamSequenceInput(240000, 5, 5000, 1000),
              updateArgs->timestamp);
          scenario->lucidLoaded = true;
          return true;
        }
      }
      break;
  }
  return false;
}


bool ncLucidAvailableMaskInstruction(ncLucidDreamScenario *scenario){
  return availableMaskInstruction(scenario);
}

ncAtomicInstruction ncLucidGetNextMaskInstruction(ncLucidDreamScenario *scenario){
  return getNextMaskInstruction(scenario);
}

static bool availableMaskInstruction(ncLucidDreamScenario *scenario){
  return !scenario->dataFiFo.empty();
}

static ncAtomicInstruction getNextMaskInstruction(BaseScenario *scenario){
  ncAtomicInstruction _instruction;
  if (scenario->dataFiFo.empty())
    std::memset(&_instruction, 0, sizeof(ncAtomicInstruction));
  else{
    _instruction = scenario->dataFiFo.front();
    scenario->dataFiFo.pop();
  }
  return _instruction;
}

static std::queue<ncAtomicInstruction> loadDefaultScenario(void){
  std::queue<ncAtomicInstruction> _queue;

  killAllDevices(_queue);

  return _queue;
}
// scenariusz Michała

typedef struct {
  int endTime;
}ncMichalScenarioArgs;

struct MichalScenario:BaseScenario{
};

std::queue<ncAtomicInstruction> *ncMilchalInitScenario (ncMichalScenarioArgs initArgs){

}

std::queue<ncAtomicInstruction> stateContextUpdate (ScenarioStates state,
    const ScenarioInput *input){

}
void ncMichalUpdateScenario (MichalScenario *scenario, const ncMichalScenarioInput *updateArgs){
  scenario->dataFiFo = stateContextUpdate(scenario->state, (const ScenarioInput *) updateArgs);
}
