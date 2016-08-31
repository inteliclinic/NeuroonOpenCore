#include "AmplitudeFilter.h"
#include <gtest/gtest.h>
#include <vector>
#include <iostream>
#include <exception>

#include <dlib/matrix.h>

struct AmplitudeFilterTest: public ::testing::Test {
	virtual void SetUp() {
	}

	virtual void TearDown() {

	}
};

TEST_F(AmplitudeFilterTest, basic_amplitude_filter_test) {
	const int rows = 20;
	const int cols = 4;
	const int column = 1;
	const double critical_value = 10;

	dlib::matrix<double> data(rows, cols);
	dlib::set_all_elements(data, 0);

	dlib::set_colm(data, column) = dlib::trans(dlib::range(0, rows - 1));
	AmplitudeFilter f(critical_value);
	dlib::matrix<double> filtered = f.transform(data, dlib::colm(data, column));

	EXPECT_EQ(filtered.nr(), data.nr());
	EXPECT_EQ(filtered.nc(), data.nc());

	for (std::size_t i = 0; i != rows; ++i) {
		bool isfinite = dlib::is_finite(dlib::rowm(filtered, i));
		if (data(i, column) <= 10) {
 			ASSERT_TRUE(isfinite);
		} else {
			ASSERT_FALSE(isfinite);
		}
	}
}
