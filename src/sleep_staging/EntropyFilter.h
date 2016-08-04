/*
 * EntropyFilter.h
 *
 *  Created on: Aug 4, 2016
 *      Author: tomek
 */

#ifndef SRC_SLEEP_STAGING_ENTROPYFILTER_H_
#define SRC_SLEEP_STAGING_ENTROPYFILTER_H_

#include <dlib/matrix.h>

class EntropyFilter {
	double m_critical_value;

public:
	EntropyFilter(double critical_value);
	virtual ~EntropyFilter();


	dlib::matrix<double> transform(const dlib::matrix<double> &data);

};

#endif /* SRC_SLEEP_STAGING_ENTROPYFILTER_H_ */
