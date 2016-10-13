/*
 * EntropyFilter.h
 *
 *  Created on: Aug 4, 2016
 *      Author: tomek
 */

#ifndef SRC_SLEEP_STAGING_ENTROPYFILTER_H_
#define SRC_SLEEP_STAGING_ENTROPYFILTER_H_

#include <dlib/matrix.h>

/**
 * This class implements a filter based on entropy it is designed to reject 
 * the samples for which the entropy is too high, i.e. those for which
 * the distribution of the spectrogram is not concentrated enough around 
 * some main frequency. The filter is designed to work with data from 
 * spectrograms and is useful especially for filtering IR LED data
 *
 */
class EntropyFilter {
	double m_critical_value;

public:
    /**
     * @param critical_value : samples with entropy greater than this value
     *                         will be rejected.
     */
	EntropyFilter(double critical_value);
	virtual ~EntropyFilter();

    /**
     * Returns a matrix that is a copy of the input matrix, except that the most
     * noisy samples (those with entropy greater than the critical value) are 
     * set to NaN
     *
     * @param data : the spectrogram to be filtered.
     */
	dlib::matrix<double> transform(const dlib::matrix<double> &data);

};

#endif /* SRC_SLEEP_STAGING_ENTROPYFILTER_H_ */
