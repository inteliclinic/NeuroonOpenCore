#include <gtest/gtest.h>
#include "dlib_utils.h"
#include <iostream>
//TODO: write tests for all functions in singal_utils header



TEST(SignalUtilsTest, DISABLED_one_entropy_test) {
	dlib::matrix<double> input(10, 1);
	dlib::set_all_elements(input, 0);
	input(0,0) = 1;

	double entr = entropy(input);
	EXPECT_DOUBLE_EQ(entr, 0);
}

TEST(SignalUtilsTest, uniform_entropy_test) {

	std::vector<int> sizes({1, 4, 16, 100, 1000});
	for (int s : sizes) {
		dlib::matrix<double> input(s, 1);
		dlib::set_all_elements(input, 1);

		double entr = entropy(input);
		double entr_exponentiated = exp(entr);
		EXPECT_FLOAT_EQ(entr_exponentiated, s);
	}
}

TEST(SignalUtilsTest, basic_logistic_test) {
	dlib::matrix<double> input = dlib::linspace(-10, 10, 1000);
	auto result = logistic(input);

	for (int i = 0; i != result.nr() - 1; ++i) {
		for (int j = 0; j != result.nc(); ++j) {
			EXPECT_TRUE(result(i,j) < 1);
			EXPECT_TRUE(result(i,j) > 0);
			EXPECT_TRUE(result(i-1, j) < result(i,j));
		}
	}
	EXPECT_EQ(result.nr(), input.nr());
	EXPECT_EQ(result.nc(), input.nc());
}
