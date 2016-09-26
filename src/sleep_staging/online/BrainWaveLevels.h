/*
 * BrainWaveLevels.h
 *
 *  Created on: Sep 26, 2016
 *      Author: tomek
 */

#ifndef SRC_SLEEP_STAGING_ONLINE_BRAINWAVELEVELS_H_
#define SRC_SLEEP_STAGING_ONLINE_BRAINWAVELEVELS_H_

#include <dlib/matrix.h>
#include <vector>
#include "Spectrogram.h"


class BrainWaveLevels {

	std::vector<double> m_borders;

public:
	BrainWaveLevels(const std::vector<double> &borders);
	virtual ~BrainWaveLevels();

	dlib::matrix<double> predict(const Spectrogram &spectrogram) const;
};

#endif /* SRC_SLEEP_STAGING_ONLINE_BRAINWAVELEVELS_H_ */
