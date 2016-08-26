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
