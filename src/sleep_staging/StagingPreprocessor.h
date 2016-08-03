/*
 * StagingPreprocessor.h
 *
 *  Created on: Jul 29, 2016
 *      Author: tomek
 */

#ifndef SRC_SLEEP_STAGING_STAGINGPREPROCESSOR_H_
#define SRC_SLEEP_STAGING_STAGINGPREPROCESSOR_H_

#include <dlib/matrix.h>

class StagingPreprocessor {
public:
	StagingPreprocessor();
	virtual ~StagingPreprocessor();

	dlib::matrix<double> transform(const dlib::matrix<double>& eeg_signal, const dlib::matrix<double>& ir_signal);

};

#endif /* SRC_SLEEP_STAGING_STAGINGPREPROCESSOR_H_ */
