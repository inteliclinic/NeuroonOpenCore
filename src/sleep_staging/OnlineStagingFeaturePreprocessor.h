/*
 * OnlineFeaturePreprocessor.h
 *
 *  Created on: Aug 24, 2016
 *      Author: tomek
 */

#ifndef SRC_SLEEP_STAGING_ONLINESTAGINGFEATUREPREPROCESSOR_H_
#define SRC_SLEEP_STAGING_ONLINESTAGINGFEATUREPREPROCESSOR_H_

#include <dlib/matrix.h>
#include "ExpandingMean.h"

class OnlineStagingFeaturePreprocessor {

const int EEG_FFT_WINDOW = 10 * 1024;
const int EEG_FFT_OVERLAP = (EEG_FFT_WINDOW * 3) / 4;

const int IR_FFT_WINDOW = EEG_FFT_WINDOW / 5;
const int IR_FFT_OVERLAP = EEG_FFT_OVERLAP / 5;

const int NUMBER_OF_FEATURES = 5;
//const int ROLLING_MEAN_WINDOW = 20;

dlib::matrix<double> compute_eeg_features(const dlib::matrix<double>& eeg_signal);
dlib::matrix<double> compute_ir_features(const dlib::matrix<double>& ir_signal);

ExpandingMean<dlib::matrix<double>> m_mean;

public:
	OnlineStagingFeaturePreprocessor();

	void reset();
	dlib::matrix<double> transform(const dlib::matrix<double>& eeg_signal, const dlib::matrix<double>& ir_signal);
};

#endif /* SRC_SLEEP_STAGING_ONLINESTAGINGFEATUREPREPROCESSOR_H_ */
