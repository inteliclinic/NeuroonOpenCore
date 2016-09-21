/*
 * EegSignalQuality.cpp
 *
 *  Created on: Sep 21, 2016
 *      Author: tomek
 */

#include "EegSignalQuality.h"
#include "EegFeatures.h"
#include <vector>

EegSignalQuality::EegSignalQuality() {
	// TODO Auto-generated constructor stub

}

EegSignalQuality::~EegSignalQuality() {
	// TODO Auto-generated destructor stub
}


//        spindle_unnormalized = get_band(original_Sxx, original_f, 10, 14)[0]
//        spindle_unnormalized = (np.sum(spindle_unnormalized, axis=0) / 256) * (2048 * 5)
//        spindle_unnormalized = np.log(spindle_unnormalized + 0.000001)
//        spindle_unnormalized = pd.Series(spindle_unnormalized)
//        spindle_unnormalized = spindle_unnormalized.rolling(window=400, center=True, min_periods=0).mean()
//

//    quality[edf[4] > 16] = 4
//    quality[edf[4] > 16.5] = 3
//    quality[edf[4] > 17.25] = 2
//    quality[edf[4] > 18] = 1
//    quality[edf[4] > 19] = 0


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
	dlib::matrix<double> band_sum = EegFeatures::sum_in_band(spectrogram, 10, 14);
	band_sum = dlib::log(band_sum);
	double sum_value = band_sum(0,0);
	int quality = power_to_quality(sum_value);
	return quality;
}

