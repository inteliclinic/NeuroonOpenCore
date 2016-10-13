/*
 * EegSignalQuality.h
 *
 *  Created on: Sep 21, 2016
 *      Author: tomek
 */

#ifndef SRC_SLEEP_STAGING_ONLINE_EEGSIGNALQUALITY_H_
#define SRC_SLEEP_STAGING_ONLINE_EEGSIGNALQUALITY_H_

#include "Spectrogram.h"

/**
 * Computes the quality of the EEG signal
 */
class EegSignalQuality {

	int power_to_quality(double power) const;

public:
	EegSignalQuality();
	virtual ~EegSignalQuality();

    /**
     * Computes the quality based on a spectrogram. Only supports
     * spectrograms with one row (i.e. one time window)
     * 
     * @return : integer value corresponding to the quality of the signal,
     * 4 is the best, 0 is the worst.
     */
	int predict(const Spectrogram& spectrogram) const;
};

#endif /* SRC_SLEEP_STAGING_ONLINE_EEGSIGNALQUALITY_H_ */
