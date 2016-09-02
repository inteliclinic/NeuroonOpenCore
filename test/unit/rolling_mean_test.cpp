#include <gtest/gtest.h>
#include <dlib/matrix.h>

#include <RollingMean.h>

TEST(RollingMeanTest, basic_case) {
	const int COLS = 10;
	dlib::matrix<double> one = dlib::ones_matrix<double>(1, 10);

	RollingMean rm(3, COLS);

	rm.feed(1 * one);
	EXPECT_FALSE(dlib::is_finite(rm.value()));
	rm.feed(2 * one);
	EXPECT_FALSE(dlib::is_finite(rm.value()));
	rm.feed(3 * one);
	EXPECT_EQ(one * 2, rm.value());
	rm.feed(4 * one);
	EXPECT_EQ(one * 3, rm.value());
	rm.feed(5 * one);
	EXPECT_EQ(one * 4, rm.value());

}
