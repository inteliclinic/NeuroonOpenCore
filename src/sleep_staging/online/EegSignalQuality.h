/*
 * EegSignalQuality.h
 *
 *  Created on: Sep 21, 2016
 *      Author: tomek
 */

#ifndef SRC_SLEEP_STAGING_ONLINE_EEGSIGNALQUALITY_H_
#define SRC_SLEEP_STAGING_ONLINE_EEGSIGNALQUALITY_H_

#include "Spectrogram.h"

class EegSignalQuality {

	int power_to_quality(double power) const;

public:
	EegSignalQuality();
	virtual ~EegSignalQuality();


	int predict(const Spectrogram& spectrogram) const;
};

#endif /* SRC_SLEEP_STAGING_ONLINE_EEGSIGNALQUALITY_H_ */
