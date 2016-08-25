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

ExpandingStd::ExpandingStd(int rows, int cols) {
	m_sum = dlib::matrix<double>(rows, cols);
	dlib::set_all_elements(m_sum, NAN);

	m_sum_of_squares = dlib::matrix<double>(rows, cols);
	dlib::set_all_elements(m_sum_of_squares, NAN);

	m_count = 0;
}

void ExpandingStd::consume(const dlib::matrix<double> &x) {
	if (!dlib::is_finite(x)) {
		return;
	}

	if (m_count == 0) {
		m_sum = x;
		m_sum_of_squares = dlib::pointwise_multiply(x,x);
	} else {
		m_sum = m_sum + x;
		m_sum_of_squares = m_sum_of_squares + dlib::pointwise_multiply(x,x);
	}
	++m_count;
}

dlib::matrix<double> ExpandingStd::value() const {
	dlib::matrix<double> m1 = (1. / m_count) * m_sum;
	dlib::matrix<double> m1_squared = dlib::pointwise_multiply(m1, m1);

	dlib::matrix<double> m2 = (1. / m_count) * m_sum_of_squares;
	dlib::matrix<double> variance = m2 - m1_squared;
	LOG(INFO) << "variance: "<< variance;
	dlib::matrix<double> std = dlib::sqrt(variance);
	return std;
}

#endif /* SRC_EXPANDINGSTD_H_ */
