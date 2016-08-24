#include <gtest/gtest.h>
#include <stdexcept>
#include "ExpandingMean.h"

TEST(ExpandingMeanTest, basic_test1) {
	ExpandingMean<double> em;
	em.consume(0);
	EXPECT_EQ(em.value(), 0);
	em.consume(1);
	EXPECT_EQ(em.value(), 0.5);
	em.consume(2);
	EXPECT_EQ(em.value(), 1);
	em.consume(3);
	EXPECT_EQ(em.value(), 1.5);
}

TEST(ExpandingMeanTest, basic_test1_dlib_matrix) {
	ExpandingMean<dlib::matrix<double>> em;
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

TEST(ExpandingMeanTest, throws_when_empty) {
	ExpandingMean<double> em;
	EXPECT_THROW(em.value(), std::logic_error);
}
