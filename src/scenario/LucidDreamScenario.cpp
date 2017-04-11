/**
 * @file    LucidDreamScenario.cpp
 * @Author  Paweł Kaźmierzewski <p.kazmierzewski@inteliclinic.com>
 * @date    April, 2017
 * @brief   Brief description
 *
 * Description
 */

#include "LucidDreamScenario.h"
#include "ic_low_level_control.h"

LucidDreamScenario::LucidDreamScenario():
  remCounter(0),
  remStartTimestamp(0),
  remDetected(false),
  remCounted(false),
  lucidLoaded(false) {}

bool LucidDreamScenario::update(const ScenarioInput *updateArgs){
  #define remDuration (updateArgs->lucidDream.timestamp - remStartTimestamp)

  if(remDetected)
    if(remDuration >= 180000
        && !remCounted){//TODO: 180000 needs to be a parameter
      remCounter++;
      remCounted = true;
    }

  if(updateArgs->lucidDream.currentSleep_stage == REM){
    if(!remDetected){
      remStartTimestamp = updateArgs->lucidDream.timestamp;
      remDetected = true;
      remCounted = false;
    }
    else{
    }
  }
  else{
    if(remDetected){
      remDetected = false;
      remCounted = false;
    }
    lucidLoaded = false;
  }

  switch(remCounter){
    case 0:
    case 1:
    case 2:
      break;
    case 3:
      if(remDuration >= 300000){//TODO: 300000 needs to ba a parameter
        if(!lucidLoaded){
          lucidDreamSequence(240000, 5, 5000, 1000, updateArgs->lucidDream.timestamp);
          lucidLoaded = true;
          return true;
        }
      }
      break;
    default:
      if(remDuration >= 600000){//TODO: 600000 needs to ba a parameter
        if(!lucidLoaded){
          lucidDreamSequence(240000, 5, 5000, 1000, updateArgs->lucidDream.timestamp);
          lucidLoaded = true;
          return true;
        }
      }
      break;
  }
  return false;
}

void LucidDreamScenario::lucidDreamSequence(unsigned long length, unsigned long numberOfActions,
      unsigned long actionDuration, unsigned long actionPeriod, unsigned long timestamp){
  ncAtomicInstruction instruction;
  uint8_t intensities[7];
  std::memset(intensities, 63, sizeof(intensities));
  size_t len;
  for(unsigned long i=0; i<numberOfActions; ++i){
    device_set_func(instruction.data, &len, DEV_POWER_LED|DEV_RIGHT_RED_LED|DEV_LEFT_RED_LED,
        FUN_TYPE_BLINK, intensities, actionDuration/100, actionPeriod/100, 0);
    instruction.time = timestamp;
    this->pushInstruction(instruction);
    timestamp += numberOfActions>1 ? length/(numberOfActions - 1) : 0;
  }
}
