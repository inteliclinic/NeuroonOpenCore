/*
 * BrainWaveLevels.cpp
 *
 *  Created on: Sep 26, 2016
 *      Author: tomek
 */

#include "BrainWaveLevels.h"
#include "EegFeatures.h"
#include "NeuroonAlgCoreApi.h"
#include <stdexcept>

BrainWaveLevels::BrainWaveLevels() {}

BrainWaveLevels::~BrainWaveLevels() {}

std::vector<brain_wave_levels_t> BrainWaveLevels::predict(const Spectrogram &spectrogram) const {

	bool normalized = true;
	std::vector<brain_wave_levels_t> result;

	dlib::matrix<double> delta = EegFeatures::sum_in_band(spectrogram, 0.1, 3, normalized);
	dlib::matrix<double> theta = EegFeatures::sum_in_band(spectrogram, 4, 7, normalized);
	dlib::matrix<double> alpha = EegFeatures::sum_in_band(spectrogram, 8, 13, normalized);
	dlib::matrix<double> beta = EegFeatures::sum_in_band(spectrogram, 16, 31, normalized);
	dlib::matrix<double> sum = delta + theta + alpha + beta;
	delta = dlib::pointwise_multiply(delta, 1 / sum);
	theta= dlib::pointwise_multiply(theta, 1 / sum);
	alpha = dlib::pointwise_multiply(alpha, 1 / sum);
	beta = dlib::pointwise_multiply(beta, 1 / sum);


	for (int i = 0; i != spectrogram.size(); ++i) {
		brain_wave_levels_t levels;
		levels.alpha = alpha(i, 0);
		levels.beta = beta(i, 0);
		levels.delta = delta(i, 0);
		levels.theta = theta(i, 0);
		result.push_back(levels);
	}

	return result;
}
