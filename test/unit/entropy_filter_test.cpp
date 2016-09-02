#include "EntropyFilter.h"
#include <gtest/gtest.h>
#include <vector>
#include <iostream>
#include <exception>

#include <dlib/matrix.h>

struct EntropyFilterTest: public ::testing::Test {
	virtual void SetUp() {
	}

	virtual void TearDown() {

	}
};

TEST_F(EntropyFilterTest, basic_entropy_filter_test) {
	const int rows = 20;
	const int cols = 4;
	const int column = 1;
	const double critical_value = 1.3; //a little less than ln(4)

	dlib::matrix<double> data(rows, cols);
	dlib::set_all_elements(data, 1);

	dlib::set_rowm(data, 0) = 0.0001;
	data(0,0) = 0.9999;

	EntropyFilter f(critical_value);
	dlib::matrix<double> filtered = f.transform(data);

	EXPECT_EQ(filtered.nr(), data.nr());
	EXPECT_EQ(filtered.nc(), data.nc());

	for (int j = 0; j != filtered.nc(); ++j) {
		EXPECT_EQ(filtered(0, j), data(0, j));
	}

	for (int i = 1; i != filtered.nr(); ++i) {
		for (int j = 0; j != filtered.nc(); ++j) {
			bool isnan = filtered(i, j) != filtered(i, j);
			EXPECT_TRUE(isnan);
		}
	}
}
