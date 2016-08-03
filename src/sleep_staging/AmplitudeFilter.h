/*
 * AmplitudeFilter.h
 *
 *  Created on: Aug 3, 2016
 *      Author: tomek
 */

#ifndef SRC_SLEEP_STAGING_AMPLITUDEFILTER_H_
#define SRC_SLEEP_STAGING_AMPLITUDEFILTER_H_

#include <dlib/matrix.h>

class AmplitudeFilter {
public:
	AmplitudeFilter(double critical_value, int column=-1);
	virtual ~AmplitudeFilter();

	dlib::matrix<double> transform(const dlib::matrix<double> &data);

private:
	double m_critical_value;
	int m_column;

	dlib::matrix<int> rows_greater_than(const dlib::matrix<double> &signal, double threshold);
};

#endif /* SRC_SLEEP_STAGING_AMPLITUDEFILTER_H_ */
