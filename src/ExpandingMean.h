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
	void consume(dlib::matrix<double> x);
	dlib::matrix<double> value() const;
};

ExpandingMean::ExpandingMean(int rows, int cols) {
	m_sum = dlib::matrix<double>(rows, cols);
	dlib::set_all_elements(m_sum, NAN);
	m_count = 0;
}

void ExpandingMean::consume(dlib::matrix<double> x) {
	if (!dlib::is_finite(x)) {
		return;
	}

	if (m_count == 0) {
		m_sum = x;
	} else {
		m_sum = m_sum + x;
	}
	++m_count;
}

dlib::matrix<double> ExpandingMean::value() const {
	return (1. / m_count) * m_sum;
}

#endif /* SRC_EXPANDINGMEAN_H_ */
