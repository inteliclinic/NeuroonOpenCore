/*
 * HeartRate.cpp
 *
 *  Created on: Sep 28, 2016
 *      Author: tomek
 */

#include "SpectrogramHeartRate.h"
#include <stdexcept>
#include <algorithm>
#include "dlib_utils.h"

SpectrogramHeartRate::SpectrogramHeartRate()
{}

SpectrogramHeartRate::~SpectrogramHeartRate() {}

std::vector<double> SpectrogramHeartRate::predict(const Spectrogram& spectrogram) {
	Spectrogram pulse_band = spectrogram.create_from_band(0.3, 2.5);
	dlib::matrix<double> m = pulse_band.compute_moment(1);

	std::vector<double> result = dlib_matrix_to_vector(m);
	for (int i = 0; i != result.size(); ++i) {
		const double SECONDS_IN_A_MINUTE = 60;
		result[i] *= SECONDS_IN_A_MINUTE;
	}
	return result;
}

