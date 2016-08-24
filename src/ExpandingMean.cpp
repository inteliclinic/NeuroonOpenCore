/*
 * ExpandingMean.cpp
 *
 *  Created on: Aug 22, 2016
 *      Author: tomek
 */

#include "ExpandingMean.h"
#include <stdexcept>

ExpandingMean::ExpandingMean() {
	m_sum = 0;
	m_count = 0;
}

void ExpandingMean::consume(double x) {
	m_sum += x;
	++m_count;
}

double ExpandingMean::value() const {
	if (m_count == 0) {
		throw std::logic_error("Expanding mean: No values consumed yet!");
	}

	return m_sum / m_count;
}



