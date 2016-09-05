#include <gtest/gtest.h>
#include <stdexcept>
#include <cmath>

#include "ExpandingStd.h"

TEST(ExpandingStdTest, basic_test1_1on1_matrix) {
	ExpandingStd es(1,1);
	dlib::matrix<double> one(1,1);
	one(0,0) = 1;

	es.consume(0 * one);
	EXPECT_EQ(es.value(), 0);
	es.consume(1.0 * one);
	EXPECT_EQ(es.value(), 0.5);
	es.consume(2 * one);
	EXPECT_DOUBLE_EQ(es.value(), dlib::sqrt_2 / dlib::sqrt_3);
	es.consume(NAN * one);
	EXPECT_DOUBLE_EQ(es.value(), dlib::sqrt_2 / dlib::sqrt_3);
}
