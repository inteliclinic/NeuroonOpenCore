/*
 * ExpandingMean.h
 *
 *  Created on: Aug 22, 2016
 *      Author: tomek
 */

#ifndef SRC_EXPANDINGMEAN_H_
#define SRC_EXPANDINGMEAN_H_

#include <stdexcept>
#include <cmath>
#include <dlib/matrix.h>

/**
 * Computes the expanding mean, i.e. the mean of all values that 
 * have occured until the current moment. This is NOT equivalent to the
 * rolling mean in which the size of the window is constant and some samples
 * get 'forgotten' over time.
 *
 * This concept is very important for the standardization used in online
 * staging algorithm
 */
class ExpandingMean {
	dlib::matrix<double> m_sum;
	int m_count;
public:
	ExpandingMean(int rows, int cols);
	void consume(const dlib::matrix<double> &x);
	dlib::matrix<double> value() const;
};

#endif /* SRC_EXPANDINGMEAN_H_ */
