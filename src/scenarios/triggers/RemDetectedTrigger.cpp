/**
 * @file    RemDetectedTrigger.cpp
 * @Author  Paweł Kaźmierzewski <p.kazmierzewski@inteliclinic.com>
 * @date    May, 2017
 * @brief   Brief description
 *
 * Description
 */

#include "RemDetectedTrigger.h"

RemDetectedTrigger::RemDetectedTrigger(ncUnixTimestamp remTresholdMs,
                                       ncUnixTimestamp remFirstOffset,
                                       ncUnixTimestamp remSecondOffset)
  : m_remDetected(false),
    m_active(false),
    m_remIsCounted(false),
    m_remCounter(0),
    m_remTreshold(remTresholdMs),
    m_firstOffset(remFirstOffset),
    m_secondOffset(remSecondOffset){}

void RemDetectedTrigger::update(const rdtInput &inp){
  auto ts = inp.first;
  auto ss = inp.second;

  // REM state guadian
  if(ss != ncSleepStage::REM){
    if(!m_remDetected) return;

    m_remDetected = m_active = m_remIsCounted = false;
    m_remDuration = 0;
    return;
  }

  // REM detection guardian
  if(!m_remDetected){
    m_remDetected = true;
    m_remFirstOccurrence = ts;
    m_remDuration = 0;
    return;
  }
  else{
    m_remDuration = ts - m_remFirstOccurrence;
  }

  // REM validity guardian
  if(m_remDuration > m_remTreshold){
    this->countREM();
    return;
  }

  return;
}

void RemDetectedTrigger::countREM(){
  if(!m_remIsCounted){
    m_remCounter++;
    m_remIsCounted = true;
  }

  switch(m_remCounter){
    case 1:
    case 2:
      break;
    case 3:
      if (m_remDuration >= m_firstOffset && !m_active) m_active = true;
      break;;
    default:
      if (m_remDuration >= m_secondOffset && !m_active) m_active = true;
      break;;
  }
  return;
}

bool RemDetectedTrigger::isActive() const{
  return m_active;
}
