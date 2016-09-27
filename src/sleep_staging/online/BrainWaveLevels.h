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
#include "NeuroonAlgCoreApi.h"
#include <vector>

class BrainWaveLevels {

public:
	BrainWaveLevels();
	virtual ~BrainWaveLevels();

	std::vector<brain_wave_levels_t> predict(const Spectrogram &spectrogram) const;
};

#endif /* SRC_SLEEP_STAGING_ONLINE_BRAINWAVELEVELS_H_ */
