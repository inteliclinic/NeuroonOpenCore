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
#include "RollingMean.h"
#include <tuple>

class OnlineStagingFeaturePreprocessor {

    const int NUMBER_OF_FEATURES = 22;

    dlib::matrix<double> compute_eeg_features(const dlib::matrix<double>& eeg_signal);
    dlib::matrix<double> compute_ir_features(const dlib::matrix<double>& ir_signal);

    void online_standardize_in_place(dlib::matrix<double> &input);

    class EegSumsFeatures {
		const int EEG_FFT_WINDOW = 10 * 1024;
		const int EEG_FFT_OVERLAP = (EEG_FFT_WINDOW * 3) / 4;

    	const int NUMBER_OF_EEG_FEATURES = 20;
    	ExpandingMean m_mean;
    	RollingMean m_rolling;
    	dlib::matrix<double> m_feature_stds;
    public:
    	EegSumsFeatures();
    	void reset();
    	std::pair<dlib::matrix<double>, int> transform(const dlib::matrix<double> &eeg_signal);
    };

    class IrFeatures {
		const int IR_FFT_WINDOW = 2048;
		const int IR_FFT_OVERLAP = (2048 *3) / 4;

    	ExpandingMean m_mean;
    	ExpandingStd m_std;
    	RollingMean m_rolling;

    public:
    	IrFeatures();
    	void reset();
    	dlib::matrix<double> transform(const dlib::matrix<double> &ir_signal);
    };

    EegSumsFeatures m_eeg_features;
    IrFeatures m_ir_features;

public:

    struct preprocessing_result_t {
    	dlib::matrix<double> features;
    	int quality;
    };

    static const int ROLLING_WINDOW_SIZE = 3;
	OnlineStagingFeaturePreprocessor();

	void reset();
	preprocessing_result_t transform(const dlib::matrix<double>& eeg_signal, const dlib::matrix<double>& ir_signal,
								   double seconds_since_start);
};

#endif /* SRC_SLEEP_STAGING_ONLINESTAGINGFEATUREPREPROCESSOR_H_ */
