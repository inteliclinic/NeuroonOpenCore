/*
 * RollingMean.cpp
 *
 *  Created on: Aug 29, 2016
 *      Author: tomek
 */

#include "RollingMean.h"
#include <cmath>
#include <iostream>

RollingMean::RollingMean(int window, int columns)
: m_window(window)
, m_columns(columns)
, m_data() {}

void RollingMean::feed(dlib::matrix<double> input) {
	m_data.push_back(input);

	if (m_data.size() > m_window) {
		m_data.erase(m_data.begin());
	}
}

dlib::matrix<double> RollingMean::value() {
	dlib::matrix<double> result = dlib::zeros_matrix<double>(1, m_columns);

	if (m_data.size() < m_window) {
		std::cout << "rolling mean nan\n";
		return NAN * result;
	}

	for (auto e : m_data) {
		result = result + e;
	}
	return (1. / m_window) * result;
}


void RollingMean::reset() {
	m_data.clear();
}
