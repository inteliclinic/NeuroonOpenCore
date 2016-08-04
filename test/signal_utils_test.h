#include <gtest/gtest.h>
#include "signal_utils.h"

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
