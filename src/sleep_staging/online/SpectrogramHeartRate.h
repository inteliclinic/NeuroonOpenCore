/*
 * HeartRate.h
 *
 *  Created on: Sep 28, 2016
 *      Author: tomek
 */

#ifndef SRC_SLEEP_STAGING_ONLINE_SPECTROGRAMHEARTRATE_H_
#define SRC_SLEEP_STAGING_ONLINE_SPECTROGRAMHEARTRATE_H_

#include <dlib/matrix.h>
#include "Spectrogram.h"

class SpectrogramHeartRate {

public:
	SpectrogramHeartRate();
	~SpectrogramHeartRate();

	std::vector<double> predict(const Spectrogram& spectrogram);

};

#endif /* SRC_SLEEP_STAGING_ONLINE_SPECTROGRAMHEARTRATE_H_ */
