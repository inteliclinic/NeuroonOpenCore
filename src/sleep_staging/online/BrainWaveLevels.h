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
#include "NeuroonSignalStreamApi.h"
#include <vector>
#include "RollingMean.h"
#include "BrainWaveLevels.h"


/**
 * Used for computing the brain wave levels to present them to the user
 */
class BrainWaveLevels {
	RollingMean m_smoother;

public:
	BrainWaveLevels();
	virtual ~BrainWaveLevels();

    /**
     * Computes the brainwave levels based on the spectrogram of the EEG signal;
     */
	std::vector<ncBrainWaveLevels> predict(const Spectrogram &spectrogram);

    /**
     * resets the state of the object to the original values (important 
     * because of the rolling mean smoothing
     */
	void reset_state() {m_smoother.reset();}
};

#endif /* SRC_SLEEP_STAGING_ONLINE_BRAINWAVELEVELS_H_ */
