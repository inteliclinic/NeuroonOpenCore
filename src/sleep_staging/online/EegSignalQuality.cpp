/*
 * EegSignalQuality.cpp
 *
 *  Created on: Sep 21, 2016
 *      Author: tomek
 */

#include "EegSignalQuality.h"
#include "Features.h"
#include <vector>

EegSignalQuality::EegSignalQuality() {}

EegSignalQuality::~EegSignalQuality() {}

int EegSignalQuality::power_to_quality(double power) const {
	static std::vector<double> borders({19, 18, 17.25, 16.5, 16});
	int result = borders.size();
	for (int i = 0; i != borders.size() - 1; ++i) {
		if (power >= borders[i]) {
			result = i;
			break;
		}
	}
	return result;
}

int EegSignalQuality::predict(const Spectrogram& spectrogram) const {
	dlib::matrix<double> band_sum = Features::sum_in_band(spectrogram, 10, 14);
	band_sum = dlib::log(band_sum);
	double sum_value = band_sum(0,0);
	int quality = power_to_quality(sum_value);
	return quality;
}
