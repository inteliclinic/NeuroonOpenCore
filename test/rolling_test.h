#include "../src/Rolling.h"

#include <gtest/gtest.h>
#include <vector>
#include <memory>
#include <functional>
#include <numeric>
#include <algorithm>

#include "test_utils.h"

using namespace std;

struct RollingTest : public ::testing::Test {

  shared_ptr< const vector<double> > rolling1;
  shared_ptr< const vector<double> > rolling2;

	virtual void SetUp() {
    rolling1.reset(new vector<double>{1,2,3,4,5,6,7,8,9,10});
    rolling2.reset(new vector<double>{1,2,-1,-0.5,100,-100,0,3});
	}

	void TearDown() {
    ;
	}
};


TEST_F(RollingTest, rolling_window_length_trivial) {
  // length of a window
  // trivial
  auto&& ret1 = Rolling(rolling1, 1).apply([](const vector<double>& v){return (double)v.size();});
  auto sizes_sp = make_shared<const vector<double> >(rolling1->size(),(double)1);
  EXPECT_EQ_VECTORS(ret1, sizes_sp);

}

TEST_F(RollingTest, rolling_window_length_left) {
  // left
  auto&& ret3l = Rolling(rolling1, RollWindow(3,RollWindow::Type::LEFT)).apply([](const vector<double>& v){return (double)v.size();});
  shared_ptr<const vector<double> > sizes3l_sp(new vector<double>{3,3,3,3,3,3,3,3,2,1});
  EXPECT_EQ_VECTORS(ret3l, sizes3l_sp);
}

TEST_F(RollingTest, rolling_window_length_center) {
  // center
  auto&& ret3c = Rolling(rolling1, RollWindow(3,RollWindow::Type::CENTER)).apply([](const vector<double>& v){return (double)v.size();});
  shared_ptr<const vector<double> > sizes3c_sp(new vector<double>{2,3,3,3,3,3,3,3,3,2});
  EXPECT_EQ_VECTORS(ret3c, sizes3c_sp);
}

TEST_F(RollingTest, rolling_window_length_right) {
  // right
  auto&& ret3r = Rolling(rolling1, RollWindow(3,RollWindow::Type::RIGHT)).apply([](const vector<double>& v){return (double)v.size();});
  shared_ptr<const vector<double> > sizes3r_sp(new vector<double>{1,2,3,3,3,3,3,3,3,3});
  EXPECT_EQ_VECTORS(ret3r, sizes3r_sp);
}

TEST_F(RollingTest, rolling_apply) {

  // trivial apply
  vector<double>&& ret = Rolling(rolling1, 1).apply([](const vector<double>& v){return v[0];});
  EXPECT_EQ_VECTORS(ret, rolling1);
}

TEST_F(RollingTest, rolling_sum) {

  // rolling sum
  vector<double>&& ret = Rolling(rolling1, 2).sum();
  shared_ptr<const vector<double> > sums_sp(new vector<double>{3,5,7,9,11,13,15,17,19,10});
  EXPECT_EQ_VECTORS(ret, sums_sp);
}

TEST_F(RollingTest, rolling_mean) {

  // rolling mean
  vector<double>&& ret = Rolling(rolling1, 2).mean();
  shared_ptr<const vector<double> > means_sp(new vector<double>{1.5,2.5,3.5,4.5,5.5,6.5,7.5,8.5,9.5,10});
  EXPECT_EQ_VECTORS(ret, means_sp);
}

// priority
TEST_F(RollingTest, rolling_priority_trivial) {

  // rolling mean
  vector<double>&& ret = Rolling(rolling1, 1).priority([](double v1, double v2){return v1 < v2;});
  EXPECT_EQ_VECTORS(ret, rolling1);
}

TEST_F(RollingTest, rolling_priority) {

  // rolling mean
  vector<double>&& ret = Rolling(rolling1, 2).priority([](double v1, double v2){return v1 > v2;});
  shared_ptr<const vector<double> > shoulds_sp(new vector<double>{2,3,4,5,6,7,8,9,10,10});
  EXPECT_EQ_VECTORS(ret, shoulds_sp);
}

TEST_F(RollingTest, rolling_min) {

  // rolling mean
  vector<double>&& ret = Rolling(rolling2, RollWindow(5,RollWindow::Type::CENTER)).min();
  shared_ptr<const vector<double> > shoulds_sp(new vector<double>{-1,-1,-1,-100,-100,-100,-100,-100});
  EXPECT_EQ_VECTORS(ret, shoulds_sp);
}

TEST_F(RollingTest, rolling_max) {

  // rolling mean
  vector<double>&& ret = Rolling(rolling2, RollWindow(3,RollWindow::Type::RIGHT)).max();
  rolling2.reset(new vector<double>{1,2,-1,-0.5,100,-100,0,3});
  shared_ptr<const vector<double> > shoulds_sp(new vector<double>{1,2,2,2,100,100,100,3});
  EXPECT_EQ_VECTORS(ret, shoulds_sp);
}
