/**
 * @file    scenario_test.cpp
 * @Author  Paweł Kaźmierzewski <p.kazmierzewski@inteliclinic.com>
 * @date    December, 2016
 * @brief   Brief description
 *
 * Description
 */

#include <gtest/gtest.h>
#include "NeuroonMaskScenariosApi.h"
#include <stdio.h>

TEST(ScenarioTest, generate_basic_scenario){
  ncLucidDreamScenarioArgs lucidScenarioArgs;
  lucidScenarioArgs.remStabilityTreshold = LRS_DEFAULT;
  lucidScenarioArgs.startingIntensity = LP_BRIGHT;
  ncLucidDreamScenario *lucidScenario = ncLucidInitScenario(lucidScenarioArgs);

  ncAtomicInstruction instruction;
  instruction = ncLucidGetNextMaskInstruction(lucidScenario);
  for(auto v:instruction.data){
    printf("0x%x\n\r", 0xFF&v);
  }
  EXPECT_EQ(100, instruction.time);
  instruction = ncLucidGetNextMaskInstruction(lucidScenario);
  EXPECT_EQ(200, instruction.time);
  instruction = ncLucidGetNextMaskInstruction(lucidScenario);
  EXPECT_EQ(300, instruction.time);
}
