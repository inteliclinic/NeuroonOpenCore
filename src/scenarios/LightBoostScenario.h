/**
 * @file    LightBoostScenario.h
 * @Author  Michał Adamczyk <m.adamczyk@inteliclinic.com>
 * @date    April, 2017
 * @brief   Brief description
 *
 * Description
 */

#ifndef LIGHTBOOSTSCENARIO_H
#define LIGHTBOOSTSCENARIO_H

#include "MacroScenario.h"
#include "NeuroonMaskScenariosApi.h"

#include "LightBoostLightSequenceScenario.h"

class LightBoostScenario : public MacroScenario {
public:

  LightBoostScenario(const ncScenarioInitArgs *args);

private:
  const int kContactDetectionTresholdMs = 6000;
  ncUnixTimestamp _start_ts = 0;

  ncLightIntensityLevel _intensity; /**< Level of intensity of led light */
  unsigned int _length_in_minutes;  /**< Length of the scenario in minutes. */

  bool _last_time_lost_contact = false;

};

#endif /* !LIGHTBOOSTSCENARIO_H */
