/*
 * StagingPreprocessor.cpp
 *
 *  Created on: Jul 29, 2016
 *      Author: tomek
 */

#include "StagingPreprocessor.h"
#include "Spectrogram.h"
#include "Features.h"
#include "AmplitudeFilter.h"
#include "Config.h"
#include <vector>
#include <cassert>
#include <iostream>
#include "dlib_utils.h"

#include "EntropyFilter.h"
StagingPreprocessor::StagingPreprocessor() {
	// TODO Auto-generated constructor stub

}

StagingPreprocessor::~StagingPreprocessor() {
	// TODO Auto-generated destructor stub
}



Spectrogram StagingPreprocessor::get_eeg_spectrogram(const dlib::matrix<double> eeg_signal) {
	Spectrogram eeg_spectrogram(eeg_signal, Config::instance().neuroon_eeg_freq(), EEG_FFT_WINDOW, EEG_FFT_OVERLAP);
	return eeg_spectrogram;
}

Spectrogram StagingPreprocessor::get_ir_spectrogram(const dlib::matrix<double> ir_signal) {
	const Spectrogram pulse_spectrogram(ir_signal, Config::instance().neuroon_ir_freq(), IR_FFT_WINDOW, IR_FFT_OVERLAP);
	return pulse_spectrogram;
}

dlib::matrix<double> StagingPreprocessor::transform(const dlib::matrix<double>& eeg_signal,
													const dlib::matrix<double>& ir_signal) {

	Spectrogram eeg_spectrogram = get_eeg_spectrogram(eeg_signal);
	Spectrogram pulse_spectrogram = get_ir_spectrogram(ir_signal);

	dlib::matrix<double> features = transform(eeg_spectrogram, pulse_spectrogram);
	return features;
}

dlib::matrix<double> StagingPreprocessor::transform(const Spectrogram &eeg_spectrogram, const Spectrogram &pulse_spectrogram) {
	std::vector<double> borders({2.5, 7.5, 10, 14, 21});
	dlib::matrix<double> eeg_sums = Features::sum_by_borders(eeg_spectrogram, borders);
	eeg_sums = dlib::log(eeg_sums);
	const double EEG_FILTER_CRITICAL = 19;
	const int EEG_FILTER_COLUMN = 2;
	AmplitudeFilter f(EEG_FILTER_CRITICAL);

	eeg_sums = f.transform(eeg_sums, dlib::colm(eeg_sums, EEG_FILTER_COLUMN));

// =================== PULSE FEATURES =================================

	const int N_MAX_FOR_SPREAD = 1;
	//TODO: filter the pulse here
	dlib::matrix<double> pulse_band = pulse_spectrogram.get_band(0.6, 1.5625);
	const double CRITICAL_PULSE_SPECTROGRAM_ENTROPY = 4.1;
	EntropyFilter pulse_filter(CRITICAL_PULSE_SPECTROGRAM_ENTROPY);
	pulse_band = pulse_filter.transform(pulse_band);
	dlib::matrix<double> n_max_to_med = Features::n_max_to_median(pulse_band, N_MAX_FOR_SPREAD);
	const int IR_ROLLING_MEAN_WINDOW = 50;
	n_max_to_med = Features::sparse_rolling_mean(n_max_to_med, IR_ROLLING_MEAN_WINDOW);
	n_max_to_med = Features::standardize(n_max_to_med);

	if (n_max_to_med.nr() != eeg_sums.nr()) {
		std::stringstream s;
		s << "StagingPreprocessor: IR and EEG features row counts don't align: "
		  << n_max_to_med.nr() << " (IR) vs. " << eeg_sums.nr() << " (EEG)" << std::endl;
		throw std::logic_error(s.str());
	}
// =================== PULSE FEATURES END =============================
	//TODO: standardize the bands

	dlib::matrix<double> features(eeg_spectrogram.size(), NUMBER_OF_FEATURES);
	int feature_index = 0;
	for (; feature_index != eeg_sums.nc(); ++feature_index) {
		dlib::set_colm(features, feature_index) = Features::sparse_rolling_mean(dlib::colm(eeg_sums, feature_index), ROLLING_MEAN_WINDOW);

		dlib::set_colm(features, feature_index) = Features::standardize(dlib::colm(features, feature_index));
	}
	std::cout << "after rolling and standardize: " << nan_ratio(features) << std::endl;

	const int SPINDLE_BAND_INDEX = 2;
	const int THETA_STANDARDIZATION_WINDOW_SIZE = 100;
	dlib::set_colm(features, feature_index) = dlib::colm(eeg_sums, SPINDLE_BAND_INDEX);
	dlib::set_colm(features, feature_index) = Features::sparse_rolling_mean(dlib::colm(features, feature_index), ROLLING_MEAN_WINDOW);
	dlib::set_colm(features, feature_index) = Features::standardize_in_window(dlib::colm(features, feature_index),
			THETA_STANDARDIZATION_WINDOW_SIZE);

	++feature_index;

	dlib::set_colm(features, feature_index) = n_max_to_med;
	++feature_index;


	dlib::matrix<double> time = eeg_spectrogram.get_timestamps();//dlib::matrix<double>(features.nr(), 1);
	//dlib::set_colm(time, 0) = dlib::trans(dlib::range(0, features.nr() - 1));
	const double TIME_SCALING_FACTOR = 15000;
	time = time / TIME_SCALING_FACTOR;
	dlib::set_colm(features, feature_index) = time;
	feature_index++;

	assert(feature_index == NUMBER_OF_FEATURES);


	return features;
}
