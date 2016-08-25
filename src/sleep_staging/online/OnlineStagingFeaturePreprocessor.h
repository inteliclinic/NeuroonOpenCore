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
#include "ExpandingStd.h"

class OnlineStagingFeaturePreprocessor {

    const int NUMBER_OF_FEATURES = 5;

    dlib::matrix<double> compute_eeg_features(const dlib::matrix<double>& eeg_signal);
    dlib::matrix<double> compute_ir_features(const dlib::matrix<double>& ir_signal);

    void online_standardize_in_place(dlib::matrix<double> &input);

    class EegSumsFeatures {
		const int EEG_FFT_WINDOW = 10 * 1024;
		const int EEG_FFT_OVERLAP = (EEG_FFT_WINDOW * 3) / 4;

    	const int NUMBER_OF_EEG_FEATURES = 4;
    	ExpandingMean m_mean;
    	dlib::matrix<double> m_feature_stds;
    public:
    	EegSumsFeatures();
    	void reset();
    	dlib::matrix<double> transform(const dlib::matrix<double> &eeg_signal);
    };

    class IrFeatures {
		const int IR_FFT_WINDOW = 2048;
		const int IR_FFT_OVERLAP = (2048 *3) / 4;

    	ExpandingMean m_mean;
    	ExpandingStd m_std;
    public:
    	IrFeatures();
    	void reset();
    	dlib::matrix<double> transform(const dlib::matrix<double> &ir_signal);
    };

    EegSumsFeatures m_eeg_features;
    IrFeatures m_ir_features;

public:
	OnlineStagingFeaturePreprocessor();

	void reset();
	dlib::matrix<double> transform(const dlib::matrix<double>& eeg_signal, const dlib::matrix<double>& ir_signal);
};

#endif /* SRC_SLEEP_STAGING_ONLINESTAGINGFEATUREPREPROCESSOR_H_ */
