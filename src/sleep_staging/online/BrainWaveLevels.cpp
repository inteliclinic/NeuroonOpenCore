/*
 * BrainWaveLevels.cpp
 *
 *  Created on: Sep 26, 2016
 *      Author: tomek
 */

#include "BrainWaveLevels.h"
#include "EegFeatures.h"
#include "NeuroonAlgCoreApi.h"


BrainWaveLevels::BrainWaveLevels() {}

BrainWaveLevels::~BrainWaveLevels() {}

brain_wave_levels_t BrainWaveLevels::predict(const Spectrogram &spectrogram) const {
	brain_wave_levels_t result;
	bool normalized = true;
	result.delta = EegFeatures::sum_in_band(spectrogram, 0.1, 3, normalized);
	result.theta = EegFeatures::sum_in_band(spectrogram, 4, 7, normalized);
	result.alpha = EegFeatures::sum_in_band(spectrogram, 8, 13, normalized);
	result.beta = EegFeatures::sum_in_band(spectrogram, 16, 31, normalized);

	double sum = result.delta + result.theta + result.alpha + result.beta;
	result.delta = result.delta / sum;
	result.theta= result.theta / sum;
	result.alpha = result.alpha / sum;
	result.beta = result.beta / sum;

	return result;
}
