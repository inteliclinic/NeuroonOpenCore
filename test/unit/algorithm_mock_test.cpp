#include <exception>
#include <gtest/gtest.h>
#include <iostream>
#include <vector>

#include "NeuroonStandaloneAlgorithmsApi.h"
#include <cmath>

struct StandaloneAlgorithmsMocksTest : public ::testing::Test {
  virtual void SetUp() {}

  virtual void TearDown() {}
};

TEST_F(StandaloneAlgorithmsMocksTest, circadian_estimation_test) {
  ncDayTimeInstant dti = {23, 50};
  ncDayTimeInstant expected = {21, 30};
  auto ret1 = ncEstimateCrossoverTime(dti);
  EXPECT_TRUE(ret1.hour == expected.hour && ret1.minute == expected.minute);

  dti = {00, 10};
  expected = {21, 50};
  ret1 = ncEstimateCrossoverTime(dti);
  // printf("2. %d:%d", ret1.hour, ret1.minute);
  EXPECT_TRUE(ret1.hour == expected.hour && ret1.minute == expected.minute);
}

TEST_F(StandaloneAlgorithmsMocksTest, circadian_calculation_test) {
  ncDayTimeInstant dti = {23, 50};
  ncDayTimeInstant expected = {0, 10};
  auto ret1 = ncCalculateCrossoverPoint(dti, nullptr, 20);
  // printf("%d:%d", ret1.hour, ret1.minute);
  EXPECT_TRUE(ret1.hour == expected.hour && ret1.minute == expected.minute);

  dti = {22, 55};
  expected = {1, 0};
  ret1 = ncCalculateCrossoverPoint(dti, nullptr, 125);
  // printf("\n2. %d:%d", ret1.hour, ret1.minute);
  EXPECT_TRUE(ret1.hour == expected.hour && ret1.minute == expected.minute);
}
