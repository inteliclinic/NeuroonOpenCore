/*
 * BrainWaveLevels.cpp
 *
 *  Created on: Sep 26, 2016
 *      Author: tomek
 */

#include "BrainWaveLevels.h"
#include "EegFeatures.h"

BrainWaveLevels::BrainWaveLevels(const std::vector<double> &borders)
: m_borders(borders) {}

BrainWaveLevels::~BrainWaveLevels() {}

dlib::matrix<double> BrainWaveLevels::predict(const Spectrogram &spectrogram) const {
	dlib::matrix<double> result = EegFeatures::sum_by_borders(spectrogram, m_borders);
	return result;
}
