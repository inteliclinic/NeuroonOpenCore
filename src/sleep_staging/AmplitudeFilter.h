/*
 * AmplitudeFilter.h
 *
 *  Created on: Aug 3, 2016
 *      Author: tomek
 */

#ifndef SRC_SLEEP_STAGING_AMPLITUDEFILTER_H_
#define SRC_SLEEP_STAGING_AMPLITUDEFILTER_H_

#include <dlib/matrix.h>


/**
 * A simple filter for spectrograms. Assigns NaN for samples that have the 
 * vaulue in filter_column greater than the critical value
 */
class AmplitudeFilter {
public:
	AmplitudeFilter(double critical_value);

    /**
     * Returns a copy of the input matrix with the noisy rows set to NaN
     * a row is considered noisy if the value in its filter_column is
     * greater than the critical value for the filter.
     */
	dlib::matrix<double> transform(const dlib::matrix<double> &data, const dlib::matrix<double> &filter_column);

private:
	double m_critical_value;
	int m_column;
};

#endif /* SRC_SLEEP_STAGING_AMPLITUDEFILTER_H_ */
