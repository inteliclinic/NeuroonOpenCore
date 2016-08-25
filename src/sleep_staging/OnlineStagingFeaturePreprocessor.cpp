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
#include "AmplitudeFilter.h"

#include <cassert>

OnlineStagingFeaturePreprocessor::OnlineStagingFeaturePreprocessor()
: m_mean(1, NUMBER_OF_FEATURES)
, m_feature_stds(1, NUMBER_OF_FEATURES)
{
	//TODO: hardcode the stds for the features
	dlib::set_all_elements(m_feature_stds, 1);

}

dlib::matrix<double> OnlineStagingFeaturePreprocessor::compute_eeg_features(const dlib::matrix<double>& eeg_signal) {
	assert(eeg_signal.nr() == EEG_FFT_WINDOW);
	assert(eeg_signal.nc() == 1);

	const int overlap = 0;
	Spectrogram eeg_spectrogram(eeg_signal, Config::instance().neuroon_eeg_freq(), EEG_FFT_WINDOW, overlap);

	std::vector<double> borders({2.5, 7.5, 10, 14, 21});
	dlib::matrix<double> band_sums = EegFeatures::sum_by_borders(eeg_spectrogram, borders);

	band_sums = dlib::log(band_sums);

	const double EEG_FILTER_CRITICAL = 19;
	const int EEG_FILTER_COLUMN = 2;
	AmplitudeFilter f(EEG_FILTER_CRITICAL, EEG_FILTER_COLUMN);
	band_sums = f.transform(band_sums);

	return band_sums;
}

dlib::matrix<double> OnlineStagingFeaturePreprocessor::compute_ir_features(const dlib::matrix<double>& ir_signal) {
	assert(ir_signal.nr() == IR_FFT_WINDOW);
	assert(ir_signal.nc() == 1);

	const int overlap = 0;
	Spectrogram ir_spectrogram(ir_signal, Config::instance().neuroon_ir_freq(), IR_FFT_WINDOW, overlap);
	dlib::matrix<double> pulse_band = ir_spectrogram.get_band(0.6, 1.5625);
	dlib::matrix<double> result (1,1);
	const int N_MAX_TO_MEDIAN_N = 3;
	result(0,0) = EegFeatures::n_max_to_median(pulse_band, N_MAX_TO_MEDIAN_N);


	return result;
}

void OnlineStagingFeaturePreprocessor::online_standardize_in_place(dlib::matrix<double> &input) {
	m_mean.consume(input);
	input = input - m_mean.value();

	input = dlib::pointwise_multiply(input, m_feature_stds);
}

dlib::matrix<double> OnlineStagingFeaturePreprocessor::transform(const dlib::matrix<double>& eeg_signal,
																 const dlib::matrix<double>& ir_signal) {
	dlib::matrix<double> result(1, NUMBER_OF_FEATURES);

	auto eeg_features = compute_eeg_features(eeg_signal);
	auto ir_features = compute_ir_features(ir_signal);

	dlib::set_colm(result, dlib::range(0, eeg_features.nc() - 1)) = eeg_features;
	dlib::set_colm(result, dlib::range(eeg_features.nc(), eeg_features.nc() + ir_features.nc() - 1)) = ir_features;

	online_standardize_in_place(result);

	//TODO: possibly add rolling mean and a delay

	return result;
}
