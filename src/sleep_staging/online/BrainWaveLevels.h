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
#include "RollingMean.h"
#include "BrainWaveLevels.h"

class BrainWaveLevels {
	RollingMean m_smoother;

public:
	BrainWaveLevels();
	virtual ~BrainWaveLevels();

	std::vector<brain_wave_levels_t> predict(const Spectrogram &spectrogram);
	void reset_state() {m_smoother.reset();}
};

#endif /* SRC_SLEEP_STAGING_ONLINE_BRAINWAVELEVELS_H_ */
