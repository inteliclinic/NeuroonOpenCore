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

/**
 * Computes the subject's heart rate from IR LED spectrogram by the 
 * 'mean frequency' method
 */
class SpectrogramHeartRate {

public:
	SpectrogramHeartRate();
	~SpectrogramHeartRate();

    /**
     * Computes the heart rate.
     *
     * @param spectrogram : IR LED signal spectrogram
     *
     * @return vector of heart rates for every time point of the spectrogram
     *
     */
	std::vector<double> predict(const Spectrogram& spectrogram);

};

#endif /* SRC_SLEEP_STAGING_ONLINE_SPECTROGRAMHEARTRATE_H_ */
