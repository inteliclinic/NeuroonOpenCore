/**
 * @file    RemDetectedTrigger.h
 * @Author  Paweł Kaźmierzewski <p.kazmierzewski@inteliclinic.com>
 * @date    May, 2017
 * @brief   Brief description
 *
 * Description
 */

#ifndef REMDETECTEDTRIGGER_H
#define REMDETECTEDTRIGGER_H

#include "NeuroonApiCommons.h"
#include "ScenarioTrigger.h"

typedef std::pair<ncUnixTimestamp, const ncSleepStage> rdtInput;

class RemDetectedTrigger : public ScenarioTrigger<const rdtInput&>{

  public:

    RemDetectedTrigger(ncUnixTimestamp remTresholdMs,
                       ncUnixTimestamp remFirstOffset,
                       ncUnixTimestamp remSecondOffset);
    void update(const rdtInput &inp) override;
    bool isActive() const override;

  private:

    void countREM();

    bool m_remDetected;
    bool m_active;
    bool m_remIsCounted;
    unsigned long m_remCounter;
    ncUnixTimestamp m_remFirstOccurrence;
    ncUnixTimestamp m_remTreshold;
    ncUnixTimestamp m_remDuration;
    ncUnixTimestamp m_firstOffset;
    ncUnixTimestamp m_secondOffset;
};

#endif /* !REMDETECTEDTRIGGER_H */
