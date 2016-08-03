/*
 * StagingPreprocessor.cpp
 *
 *  Created on: Jul 29, 2016
 *      Author: tomek
 */

#include "StagingPreprocessor.h"
#include "Spectrogram.h"
#include "EegFeatures.h"
#include "AmplitudeFilter.h"
#include "Config.h"
#include "signal_utils.h"
#include <vector>
#include <cassert>
#include <iostream>
StagingPreprocessor::StagingPreprocessor() {
	// TODO Auto-generated constructor stub

}

StagingPreprocessor::~StagingPreprocessor() {
	// TODO Auto-generated destructor stub
}


std::vector<std::pair<double, double>> create_bands(const std::vector<double>& borders) {
	std::vector<std::pair<double,double>> result(borders.size() - 1);
	for (std::size_t i = 0; i != borders.size() - 1; ++i) {
		result[i] = std::pair<double, double>(borders[i], borders[i+1]);
	}
	return result;
}

dlib::matrix<double> StagingPreprocessor::transform(const dlib::matrix<double>& eeg_signal,
													const dlib::matrix<double>& ir_signal) {

	const int NUMBER_OF_FEATURES = 7;

	const int EEG_FFT_WINDOW = 10 * 1024;
	const int EEG_FFT_OVERLAP = (EEG_FFT_WINDOW * 3) / 4;

	const int IR_FFT_WINDOW = EEG_FFT_WINDOW / 5;
	const int IR_FFT_OVERLAP = EEG_FFT_OVERLAP / 5;

	const int ROLLING_MEAN_WINDOW = 20;

	std::vector<double> borders({2.5, 7.5, 10, 14, 21});
	Spectrogram eeg_spectrogram(eeg_signal, Config::instance().neuroon_eeg_freq(), EEG_FFT_WINDOW, EEG_FFT_OVERLAP);

	dlib::matrix<double> eeg_sums = EegFeatures::sum_in_bands(eeg_spectrogram, create_bands(borders));
	eeg_sums = dlib::log(eeg_sums);
	const double EEG_FILTER_CRITICAL = 19;
	const int EEG_FILTER_COLUMN = 2;
	AmplitudeFilter f(EEG_FILTER_CRITICAL, EEG_FILTER_COLUMN);

	//std::cout << eeg_sums;
	std::cout << "before filter: " << nan_ratio(eeg_sums) << std::endl;
	eeg_sums = f.transform(eeg_sums);
	std::cout << "after filter: " << nan_ratio(eeg_sums) << std::endl;

	const Spectrogram pulse_spectrogram(ir_signal, Config::instance().neuroon_ir_freq(), IR_FFT_WINDOW, IR_FFT_OVERLAP);
	const int N_MAX_FOR_SPREAD = 3;
	//TODO: filter the pulse here
	dlib::matrix<double> n_max_to_med = EegFeatures::n_max_to_median(pulse_spectrogram.data(), N_MAX_FOR_SPREAD);
	n_max_to_med = EegFeatures::standardize(n_max_to_med);
	n_max_to_med = EegFeatures::sparse_rolling_mean(n_max_to_med, ROLLING_MEAN_WINDOW);

	if (n_max_to_med.nr() != eeg_sums.nr()) {
		std::stringstream s;
		s << "StagingPreprocessor: IR and EEG features row counts don't align: "
		  << n_max_to_med.nr() << " (IR) vs. " << eeg_sums.nr() << " (EEG)" << std::endl;
		throw std::logic_error(s.str());
	}

	//TODO: standardize the bands

	dlib::matrix<double> features(eeg_spectrogram.size(), NUMBER_OF_FEATURES);
	int feature_index = 0;
	for (; feature_index != eeg_sums.nc(); ++feature_index) {
		dlib::set_colm(features, feature_index) = EegFeatures::sparse_rolling_mean(EegFeatures::standardize(dlib::colm(eeg_sums, feature_index)),
																ROLLING_MEAN_WINDOW);
	}
	std::cout << "after rolling and standardize: " << nan_ratio(features) << std::endl;

	const int SPINDLE_BAND_INDEX = 2;
	const int THETA_STANDARDIZATION_WINDOW_SIZE = 100;
	dlib::set_colm(features, feature_index) = EegFeatures::standardize_in_window(dlib::colm(eeg_sums, SPINDLE_BAND_INDEX),
			THETA_STANDARDIZATION_WINDOW_SIZE);
	dlib::set_colm(features, feature_index) = EegFeatures::sparse_rolling_mean(dlib::colm(features, feature_index), ROLLING_MEAN_WINDOW);


	++feature_index;

	dlib::set_colm(features, feature_index) = n_max_to_med;
	++feature_index;

	dlib::matrix<double> time = dlib::matrix<double>(features.nr(), 1);
	dlib::set_colm(time, 0) = dlib::trans(dlib::range(0, features.nr() - 1));
	const double TIME_SCALING_FACTOR = 15000;
	time = time / TIME_SCALING_FACTOR;
	dlib::set_colm(features, feature_index) = time;
	feature_index++;

	assert(feature_index == NUMBER_OF_FEATURES);

	//TODO: additional feature - standardize the spindle bandwidth in a window (100 spectrogram rows?)

	//TODO: compute the time

	//TODO: combine and return the result matrix
	return features;
}
