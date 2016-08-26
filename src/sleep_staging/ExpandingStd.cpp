#include "ExpandingStd.h"

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
	LOG(DEBUG) << "variance: "<< variance;
	dlib::matrix<double> std = dlib::sqrt(variance);
	return std;
}
