/*
 * HeartRate.cpp
 *
 *  Created on: Sep 28, 2016
 *      Author: tomek
 */

#include "SpectrogramHeartRate.h"
#include <stdexcept>
#include <algorithm>
SpectrogramHeartRate::SpectrogramHeartRate()
{}

SpectrogramHeartRate::~SpectrogramHeartRate() {}

std::vector<double> SpectrogramHeartRate::predict(const Spectrogram& spectrogram) {
	std::vector<double> result;
	result.push_back(0);
	return result;
}

