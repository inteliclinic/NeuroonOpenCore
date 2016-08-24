/*
 * OnlineFeaturePreprocessor.cpp
 *
 *  Created on: Aug 24, 2016
 *      Author: tomek
 */

#include "OnlineStagingFeaturePreprocessor.h"
#include "Spectrogram.h"
#include "Config.h"
#include "EegFeatures.h"

#include <cassert>

OnlineStagingFeaturePreprocessor::OnlineStagingFeaturePreprocessor()
: m_mean() {}

dlib::matrix<double> OnlineStagingFeaturePreprocessor::compute_eeg_features(const dlib::matrix<double>& eeg_signal) {
	assert(eeg_signal.nc() == 1);
	assert(eeg_signal.nr() == EEG_FFT_WINDOW);

	const int overlap = 0;
	Spectrogram eeg_spectrogram(eeg_signal, Config::instance().neuroon_eeg_freq(), EEG_FFT_WINDOW, overlap);

	std::vector<double> borders({2.5, 7.5, 10, 14, 21});
	dlib::matrix<double> band_sums = EegFeatures::sum_by_borders(eeg_spectrogram, borders);

	return band_sums;
}

dlib::matrix<double> OnlineStagingFeaturePreprocessor::compute_ir_features(const dlib::matrix<double>& ir_signal) {
	assert(ir_signal.nc() == 1);
	assert(ir_signal.nr() == IR_FFT_WINDOW);

	const int overlap = 0;
	Spectrogram ir_spectrogram(ir_signal, Config::instance().neuroon_ir_freq(), IR_FFT_WINDOW, overlap);

	dlib::matrix<double> dummy_result(1, 1);
	return dummy_result;
}

dlib::matrix<double> OnlineStagingFeaturePreprocessor::transform(const dlib::matrix<double>& eeg_signal, const dlib::matrix<double>& ir_signal) {
	dlib::matrix<double> result(1, NUMBER_OF_FEATURES);

	auto eeg_features = compute_eeg_features(eeg_signal);
	auto ir_features = compute_ir_features(ir_signal);



	dlib::set_colm(result, dlib::range(0, eeg_features.nc() - 1)) = eeg_features;
	dlib::set_colm(result, dlib::range(eeg_features.nc(), eeg_features.nc() + ir_features.nc() - 1)) = ir_features;
	return result;
}
