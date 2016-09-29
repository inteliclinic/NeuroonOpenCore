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
	brain_wave_levels_t* brain_waves;
	int bw_size;
	double heart_rate;
	double* pulse_data;
	int pd_size;
};


class OnlinePresentationAlgorithm : public SinkStreamingAlgorithm<OnlinePresentationResult> {

	std::vector<brain_wave_levels_t> m_bw;
	std::vector<double> m_pulse_data;
	double m_heart_rate;

	int m_last_eeg_index;
	int m_last_ir_index;
	bool m_active;

	void process_brain_waves(const INeuroonSignals & input);
	void process_pulseoximetry(const INeuroonSignals & input);

	void update_pulseoximeter_data(const INeuroonSignals & input);
	void update_heart_rate(const INeuroonSignals & input);
	void update_results();

public:

	typedef IDataSink<OnlinePresentationResult> sink_t;

	OnlinePresentationAlgorithm(const std::vector<sink_t*> & sinks);
	virtual ~OnlinePresentationAlgorithm();

	virtual void reset_state() override;
	virtual void process_input(const INeuroonSignals & input) override;
	virtual void end_streaming(const INeuroonSignals & input) override;

	void activate();
	void deactivate();

private:
};

#endif /* SRC_SLEEP_STAGING_OnlinePresentationAlgorithm_H_ */
