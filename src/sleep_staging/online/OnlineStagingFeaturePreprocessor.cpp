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
#include "EntropyFilter.h"
#include "dlib_utils.h"
#include <tuple>
#include <cassert>
#include <utility>
#include "EegSignalQuality.h"

OnlineStagingFeaturePreprocessor::OnlineStagingFeaturePreprocessor()
{

}

OnlineStagingFeaturePreprocessor::EegSumsFeatures::EegSumsFeatures()
: m_feature_stds(1, NUMBER_OF_EEG_FEATURES),
  m_mean(1, NUMBER_OF_EEG_FEATURES),
  m_rolling(ROLLING_WINDOW_SIZE, NUMBER_OF_EEG_FEATURES)

{
	dlib::set_all_elements(m_feature_stds, 0.3);
}

dlib::matrix<double>
OnlineStagingFeaturePreprocessor::EegSumsFeatures::transform(const Spectrogram& eeg_spectrogram) {
//	assert(eeg_signal.nr() == EEG_FFT_WINDOW);
//	assert(eeg_signal.nc() == 1);
//
//	const int overlap = 0;
//	Spectrogram eeg_spectrogram(eeg_signal, Config::instance().neuroon_eeg_freq(), EEG_FFT_WINDOW, overlap);

	std::vector<double> borders({ 1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17,
							       18, 19, 20, 21});
	dlib::matrix<double> band_sums = EegFeatures::sum_by_borders(eeg_spectrogram, borders, true);
	m_rolling.feed(band_sums);
	band_sums = m_rolling.value();
	band_sums = dlib::log(band_sums);

	const double EEG_FILTER_CRITICAL = 19;
	dlib::matrix<double> filter_band = EegFeatures::sum_in_band(eeg_spectrogram, 10, 14, false);
	filter_band = dlib::log(filter_band);
	AmplitudeFilter f(EEG_FILTER_CRITICAL);
	band_sums = f.transform(band_sums, filter_band);

	m_mean.consume(band_sums);
	band_sums = standardize(band_sums, m_mean.value(), m_feature_stds);

	return band_sums;
}

OnlineStagingFeaturePreprocessor::IrFeatures::IrFeatures()
: m_mean(1,1)
, m_std(1,1)
, m_rolling(OnlineStagingFeaturePreprocessor::ROLLING_WINDOW_SIZE, 1)
{}


dlib::matrix<double> OnlineStagingFeaturePreprocessor::IrFeatures::transform(const Spectrogram &ir_spectrogram) {
//	assert(ir_signal.nr() == IR_FFT_WINDOW);
//	assert(ir_signal.nc() == 1);
//
//	const int overlap = 0;
//	Spectrogram ir_spectrogram(ir_signal, Config::instance().neuroon_ir_freq(), IR_FFT_WINDOW, overlap);
	dlib::matrix<double> pulse_band = ir_spectrogram.get_band(0.6, 1.5502);

	const double CRITICAL_PULSE_SPECTROGRAM_ENTROPY = 4.3;
	EntropyFilter pulse_filter(CRITICAL_PULSE_SPECTROGRAM_ENTROPY);
	pulse_band = pulse_filter.transform(pulse_band);

	const int N_MAX_TO_MEDIAN_N = 3;
	dlib::matrix<double> result = EegFeatures::n_max_to_median(pulse_band, N_MAX_TO_MEDIAN_N);
	m_rolling.feed(result);
	result = m_rolling.value();
	m_mean.consume(result);
	m_std.consume(result);

	result = standardize(result, m_mean.value(), m_std.value());

	return result;
}

OnlineStagingFeaturePreprocessor::preprocessing_result_t
OnlineStagingFeaturePreprocessor::transform(const Spectrogram& eeg_spectrogram,
											const Spectrogram& ir_spectrogram,
											double seconds_since_start) {

	preprocessing_result_t result;
	dlib::matrix<double> features(1, NUMBER_OF_FEATURES);

	auto eeg_preprocessing = m_eeg_features.transform(eeg_spectrogram);
	auto eeg_features = eeg_preprocessing;

	auto ir_features = m_ir_features.transform(ir_spectrogram);

	dlib::set_colm(features, dlib::range(0, eeg_features.nc() - 1)) = eeg_features;
	dlib::set_colm(features, dlib::range(eeg_features.nc(), eeg_features.nc() + ir_features.nc() - 1)) = ir_features;

	//ugly hack that makes it exactly as in scipy's spectrogram
	double beginning_feature = (seconds_since_start <= 45 * 60) ? 1 : 0;

	dlib::set_colm(features, NUMBER_OF_FEATURES-1) = beginning_feature;
	result.features = features;
	return result;
}
