/**
 * @file    MocScenario.h
 * @Author  Paweł Kaźmierzewski <p.kazmierzewski@inteliclinic.com>
 * @date    May, 2017
 * @brief   Brief description
 *
 * Description
 */

#ifndef MOCSCENARIO_H
#define MOCSCENARIO_H

#include "MacroScenario.h"
#include "NeuroonMaskScenariosApi.h"

class MocScenario : public MacroScenario {
  public:
    MocScenario(const ncScenarioInitArgs *args, ncScenarioType scenario);
    ncUpdateOutput update(const ncScenarioInput *inp) override;

  private:
    const ncUnixTimestamp kLucidDelta = 10000;
    const ncUnixTimestamp kLightBoostDelta = 9000;
    const ncUnixTimestamp kSleepDelta = 8000;
    const ncUnixTimestamp kPowerNapDelta = 7000;
    const ncUnixTimestamp kCircadianDelta = 6000;

    const long kNumberOfSeq = 10;

    ncUnixTimestamp m_deltaTime;
    ncUnixTimestamp m_prevTime;
    ncScenarioType m_scenario;

    long m_seqCounter;
};

#endif /* !MOCSCENARIO_H */
