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

TEST_F(StandaloneAlgorithmsMocksTest, compute_napping_schedule) {
  auto sch = compute_napping_schedule({6, 00}, 0, {}, 0, {}, 0);
  EXPECT_EQ(sch.power_nap_interval_start.hour, 14);
  EXPECT_EQ(sch.power_nap_interval_start.minute, 00);
  EXPECT_EQ(sch.ultimate_nap_interval_end.hour, 17);
  EXPECT_EQ(sch.ultimate_nap_interval_end.minute, 30);
}

TEST_F(StandaloneAlgorithmsMocksTest, create_and_delete_jetlag_state) {

  auto fl_date = 10000000;
  auto jtl_state = ncCreateJetlagTherapy(fl_date, 11000000, FD_EASTWARDS, 8);
  auto jtl_info = ncGetJetlagTherapyInfo(jtl_state);
  EXPECT_EQ(jtl_info.therapy_start, fl_date - 24*3600);
  ncDestroyJetlagTherapy(jtl_state);
}


// TEST_F(StandaloneAlgorithmsMocksTest, jet_lag_serialization) {
// TODO
// }


// TEST_F(StandaloneAlgorithmsMocksTest, jet_lat_get_event) {
// TODO
// }
