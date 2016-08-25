/*
 * StagingPreprocessor.h
 *
 *  Created on: Jul 29, 2016
 *      Author: tomek
 */

#ifndef SRC_SLEEP_STAGING_STAGINGPREPROCESSOR_H_
#define SRC_SLEEP_STAGING_STAGINGPREPROCESSOR_H_

#include <dlib/matrix.h>
#include "Spectrogram.h"

class StagingPreprocessor {


	const int EEG_FFT_WINDOW = 10 * 1024;
	const int EEG_FFT_OVERLAP = (EEG_FFT_WINDOW * 3) / 4;

	const int IR_FFT_WINDOW = EEG_FFT_WINDOW / 5;
	const int IR_FFT_OVERLAP = EEG_FFT_OVERLAP / 5;

	const int NUMBER_OF_FEATURES = 7;
	const int ROLLING_MEAN_WINDOW = 20;


public:
	StagingPreprocessor();
	virtual ~StagingPreprocessor();

	dlib::matrix<double> transform(const dlib::matrix<double>& eeg_signal, const dlib::matrix<double>& ir_signal);
	dlib::matrix<double> transform(const Spectrogram &eeg_spectrogram, const Spectrogram &pulse_spectrogram);

	Spectrogram get_eeg_spectrogram(const dlib::matrix<double> eeg_signal);
	Spectrogram get_ir_spectrogram(const dlib::matrix<double> ir_signal);
};

#endif /* SRC_SLEEP_STAGING_STAGINGPREPROCESSOR_H_ */
