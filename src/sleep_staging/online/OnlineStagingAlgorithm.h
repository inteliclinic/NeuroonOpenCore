/*
 * OnlineStagingAlgorithm.h
 *
 *  Created on: Aug 23, 2016
 *      Author: tomek
 */

#ifndef SRC_SLEEP_STAGING_ONLINESTAGINGALGORITHM_H_
#define SRC_SLEEP_STAGING_ONLINESTAGINGALGORITHM_H_

#include "StreamingAlgorithm.h"
#include "OnlineStagingFeaturePreprocessor.h"

class OnlineStagingAlgorithm {

public:
	OnlineStagingAlgorithm();
	virtual ~OnlineStagingAlgorithm();

	void reset_state();
	void process_input(const NeuroonSignals & input);
	void stop();
};

#endif /* SRC_SLEEP_STAGING_ONLINESTAGINGALGORITHM_H_ */
