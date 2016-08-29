/*
 * RollingMean.h
 *
 *  Created on: Aug 29, 2016
 *      Author: tomek
 */

#ifndef SRC_NUMERICS_ROLLINGMEAN_H_
#define SRC_NUMERICS_ROLLINGMEAN_H_

#include <dlib/matrix.h>
#include <vector>

class RollingMean {

	int m_window;
	int m_columns;
	std::vector<dlib::matrix<double>> m_data;

public:
	RollingMean(int window, int columns);

	void feed(dlib::matrix<double> input);
	dlib::matrix<double> value();
};

#endif /* SRC_NUMERICS_ROLLINGMEAN_H_ */
