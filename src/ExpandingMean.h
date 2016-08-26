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

class ExpandingMean {
	dlib::matrix<double> m_sum;
	int m_count;
public:
	ExpandingMean(int rows, int cols);
	void consume(const dlib::matrix<double> &x);
	dlib::matrix<double> value() const;
};

#endif /* SRC_EXPANDINGMEAN_H_ */
