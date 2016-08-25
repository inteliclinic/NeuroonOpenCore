/*
 * ExpandingMean.h
 *
 *  Created on: Aug 22, 2016
 *      Author: tomek
 */

#ifndef SRC_EXPANDINGMEAN_H_
#define SRC_EXPANDINGMEAN_H_

#include <stdexcept>

template <typename T>
class ExpandingMean {
	T m_sum;
	int m_count;
public:
	ExpandingMean();
	void consume(T x);
	T value() const;
};

template <typename T>
ExpandingMean<T>::ExpandingMean() {
	m_sum = T();
	m_count = 0;
}

template <typename T>
void ExpandingMean<T>::consume(T x) {
	if (m_count == 0) {
		m_sum = x;
	} else {
		m_sum = m_sum + x;
	}
	++m_count;
}

template <typename T>
T ExpandingMean<T>::value() const {
	if (m_count == 0) {
		throw std::logic_error("Expanding mean: No values consumed yet!");
	}
	return (1. / m_count) * m_sum;
}

#endif /* SRC_EXPANDINGMEAN_H_ */
