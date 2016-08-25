#include <gtest/gtest.h>
#include <stdexcept>
#include <cmath>

#include "ExpandingMean.h"

TEST(ExpandingMeanTest, basic_test1_1on1_matrix) {
	ExpandingMean em(1,1);
	dlib::matrix<double> one(1,1);
	one(0,0) = 1;

	em.consume(0 * one);
	EXPECT_EQ(em.value(), 0);
	em.consume(1 * one);
	EXPECT_EQ(em.value(), 0.5);
	em.consume(2 * one);
	EXPECT_EQ(em.value(), 1);
	em.consume(3 * one);
	EXPECT_EQ(em.value(), 1.5);
}

TEST(ExpandingMeanTest, basic_test1_dlib_matrix) {
	ExpandingMean em(2, 1);
	dlib::matrix<double> mat(2,1);
	dlib::set_all_elements(mat, 1);

	em.consume(mat);
	//EXPECT_EQ(em.value(), 0);
	em.consume(mat);
	//EXPECT_EQ(em.value(), 0.5);
	em.consume(mat);
	//EXPECT_EQ(em.value(), 1);
	em.consume(mat);
	//EXPECT_EQ(em.value(), 1.5);
}

TEST(ExpandingMeanTest, nan_when_empty) {
	ExpandingMean em(1,1);
	EXPECT_TRUE(!dlib::is_finite(em.value()));
}


TEST(ExpandingMeanTest, ignores_matrix_with_nans) {
	ExpandingMean em(2,1);
	dlib::matrix<double> mat(2,1);
	dlib::set_all_elements(mat, 1);

	dlib::matrix<double> nan_row(2,1);
	dlib::set_all_elements(nan_row, NAN);

	em.consume(mat);
	EXPECT_EQ(em.value(), mat);
	em.consume(nan_row);
	EXPECT_EQ(em.value(), mat);
	em.consume(nan_row);
	EXPECT_EQ(em.value(), mat);
	em.consume(mat);
	EXPECT_EQ(em.value(), mat);
	em.consume(4 * mat);
	EXPECT_EQ(em.value(), 2 * mat);
	em.consume(nan_row);
	EXPECT_EQ(em.value(), 2 * mat);
}
