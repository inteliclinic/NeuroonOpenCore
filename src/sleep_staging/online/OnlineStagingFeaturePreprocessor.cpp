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

#include <cassert>

OnlineStagingFeaturePreprocessor::OnlineStagingFeaturePreprocessor()
{

}

OnlineStagingFeaturePreprocessor::EegSumsFeatures::EegSumsFeatures()
: m_feature_stds(1, NUMBER_OF_EEG_FEATURES),
  m_mean(1, NUMBER_OF_EEG_FEATURES)
{
	m_feature_stds(0,0) = 0.34;
	m_feature_stds(0,1) = 0.26;
	m_feature_stds(0,2) = 0.27;
	m_feature_stds(0,3) = 0.21;
}

dlib::matrix<double> OnlineStagingFeaturePreprocessor::EegSumsFeatures::transform(const dlib::matrix<double>& eeg_signal) {
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

	m_mean.consume(band_sums);
	band_sums = standardize(band_sums, m_mean.value(), m_feature_stds);

	return band_sums;
}

OnlineStagingFeaturePreprocessor::IrFeatures::IrFeatures()
: m_mean(1,1)
, m_std(1,1)
{}


dlib::matrix<double> OnlineStagingFeaturePreprocessor::IrFeatures::transform(const dlib::matrix<double>& ir_signal) {
	assert(ir_signal.nr() == IR_FFT_WINDOW);
	assert(ir_signal.nc() == 1);

	const int overlap = 0;
	Spectrogram ir_spectrogram(ir_signal, Config::instance().neuroon_ir_freq(), IR_FFT_WINDOW, overlap);
	dlib::matrix<double> pulse_band = ir_spectrogram.get_band(0.6, 1.5502);

	const double CRITICAL_PULSE_SPECTROGRAM_ENTROPY = 4.3;
	EntropyFilter pulse_filter(CRITICAL_PULSE_SPECTROGRAM_ENTROPY);
	pulse_band = pulse_filter.transform(pulse_band);

	const int N_MAX_TO_MEDIAN_N = 3;
	dlib::matrix<double> result = EegFeatures::n_max_to_median(pulse_band, N_MAX_TO_MEDIAN_N);

	m_mean.consume(result);
	m_std.consume(result);

	result = standardize(result, m_mean.value(), m_std.value());

	return result;
}

dlib::matrix<double> OnlineStagingFeaturePreprocessor::transform(const dlib::matrix<double>& eeg_signal,
																 const dlib::matrix<double>& ir_signal,
																 double seconds_since_start) {
	dlib::matrix<double> result(1, NUMBER_OF_FEATURES);

	auto eeg_features = m_eeg_features.transform(eeg_signal);
	auto ir_features = m_ir_features.transform(ir_signal);

	dlib::set_colm(result, dlib::range(0, eeg_features.nc() - 1)) = eeg_features;
	dlib::set_colm(result, dlib::range(eeg_features.nc(), eeg_features.nc() + ir_features.nc() - 1)) = ir_features;

	//ugly hack that makes it exactly as in scipy's spectrogram
	double beginning_feature = (seconds_since_start <= 45 * 60) ? 1 : 0;

	dlib::set_colm(result, NUMBER_OF_FEATURES-1) = beginning_feature;
	//TODO: possibly add a rolling mean and a delay
	return result;
}
