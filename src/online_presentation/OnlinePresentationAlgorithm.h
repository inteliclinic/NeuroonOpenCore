/*
 * OnlinePresentationAlgorithm.h
 *
 *  Created on: Aug 23, 2016
 *      Author: tomek
 */

#ifndef SRC_OnlinePresentationAlgorithm_H_
#define SRC_OnlinePresentationAlgorithm_H_

#include "StreamingAlgorithm.h"
#include "OnlineStagingClassifier.h"
#include "CommonTypes.h"
#include <vector>
#include <stdexcept>

#include "NeuroonAlgCoreApi.h"

struct OnlinePresentationResult {
	presentation_element_t* data;
	int size;
};


class OnlinePresentationAlgorithm : public SinkStreamingAlgorithm<OnlinePresentationResult> {

	std::vector<presentation_element_t> m_buffer;
	int m_last_eeg_index;


public:

	typedef IDataSink<OnlinePresentationResult> sink_t;

	OnlinePresentationAlgorithm(const std::vector<sink_t*> & sinks);
	virtual ~OnlinePresentationAlgorithm();

	virtual void reset_state() override;
	virtual void process_input(const INeuroonSignals & input) override;
	virtual void end_streaming(const INeuroonSignals & input) override;

private:
};

#endif /* SRC_SLEEP_STAGING_OnlinePresentationAlgorithm_H_ */
