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
#include "Spectrogram.h"
#include <tuple>

/**
 * Performs feature extraction for the online staging algorithm
 */
class OnlineStagingFeaturePreprocessor {

  /**
   * The total number of features. To be updated manually. Has to agree with
   * the number of input neurons of the MLP classifier
   */
    const int NUMBER_OF_FEATURES = 22;

    /**
     * Computes the EEG features, i.e. the sums of amplitudes 
     * in each frequency band
     */ 
    dlib::matrix<double> compute_eeg_features(const dlib::matrix<double>& eeg_signal);

    /**
     * Computes the features from the IR LED data
     */ 
    dlib::matrix<double> compute_ir_features(const dlib::matrix<double>& ir_signal);
    
    /**
     * Private helper class for computing EEG features. Can be refactored to 
     * a separate file if grows too large.
     */
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
    	dlib::matrix<double> transform(const Spectrogram& eeg_spectrogram);
    };

    /**
     * Private helper class for computing IR LED features. Can be refactored
     * to a separate file if grows too large.
     */
    class IrFeatures {
		const int IR_FFT_WINDOW = 2048;
		const int IR_FFT_OVERLAP = (2048 *3) / 4;

    	ExpandingMean m_mean;
    	ExpandingStd m_std;
    	RollingMean m_rolling;

    public:
    	IrFeatures();
    	void reset();
    	dlib::matrix<double> transform(const Spectrogram& ir_spectrogram);
    };

    EegSumsFeatures m_eeg_features;
    IrFeatures m_ir_features;

public:
	OnlineStagingFeaturePreprocessor();

    struct preprocessing_result_t {
    	dlib::matrix<double> features;
    };

    /**
     * The size of the rolling window used
     */
    static const int ROLLING_WINDOW_SIZE = 3;

    /**
     * Resets the preprocessor to the original state.
     */ 
	void reset();

    /**
     * Transforms the  EEG and IR spectrograms to classification features  
     * @param eeg_spectrogram -- the spectrogram of the EEG signal
     * @param ir_spectrogram -- the spectrogram of IR LED signal, has to have
     * as much rows as the EEG spectrogram (i.e. the size of the window has to
     * be adjusted to compensate for the lower sampling rate of the IR signal
     * @param seconds_since_start -- number of seconds that passed from the beginning of sleep. Used to compute one of the classification features based on time.
     */
	preprocessing_result_t transform(const Spectrogram& eeg_spectrogram, const Spectrogram& ir_spectrogram,
								   double seconds_since_start);
};

#endif /* SRC_SLEEP_STAGING_ONLINESTAGINGFEATUREPREPROCESSOR_H_ */
