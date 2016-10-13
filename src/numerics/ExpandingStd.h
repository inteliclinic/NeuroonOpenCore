/*
 * ExpandingStd.h
 *
 *  Created on: Aug 22, 2016
 *      Author: tomek
 */

#ifndef SRC_EXPANDINGSTD_H_
#define SRC_EXPANDINGSTD_H_

#include <stdexcept>
#include <cmath>
#include <dlib/matrix.h>
#include "logger.h"

/**
 * Computes the expanding standard deviation, i.e. the std of all values that 
 * have occured until the current moment. This is NOT equivalent to the
 * rolling std in which the size of the window is constant and some samples
 * get 'forgotten' over time.
 *
 * This concept is very important for the standardization used in online
 * staging algorithm
 */

class ExpandingStd {
	dlib::matrix<double> m_sum;
	dlib::matrix<double> m_sum_of_squares;
	int m_count;
public:
	ExpandingStd(int rows, int cols);
	void consume(const dlib::matrix<double> &x);
	dlib::matrix<double> value() const;
};

#endif /* SRC_EXPANDINGSTD_H_ */
