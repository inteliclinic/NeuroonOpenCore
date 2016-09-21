/*
 * OnlineStagingAlgorithm.cpp
 *
 *  Created on: Aug 23, 2016
 *      Author: tomek
 */

#include "OnlineStagingAlgorithm.h"
#include "MlpClassifier.h"
#include <vector>
#include <dlib/matrix.h>
#include <cassert>
#include "dlib_utils.h"
#include "logger.h"

OnlineStagingAlgorithm::OnlineStagingAlgorithm(const std::vector<OnlineStagingAlgorithm::sink_t*> & sinks)
: SinkStreamingAlgorithm<SleepStagingResult>(sinks)
{
	m_last_eeg_index = 0;
	m_last_ir_index = 0;

	m_first_timestamp = 0;
}

OnlineStagingAlgorithm::~OnlineStagingAlgorithm() {

}

void OnlineStagingAlgorithm::reset_state() {
	m_model.reset();
}

void OnlineStagingAlgorithm::process_input(const INeuroonSignals & input) {
	ullong eeg_sample_index = input.total_signal_samples(EEG);
	ullong ir_sample_index = input.total_signal_samples(IR_LED);

	if (m_last_eeg_index + EEG_INTERVAL > eeg_sample_index
		&& m_last_ir_index + IR_INTERVAL > ir_sample_index) {

		//nothing to be done yet -- not enough data
		return;
	}

	if (input.eeg_signal().size() < EEG_WINDOW || input.ir_led_signal().size() < IR_WINDOW) {
		return;
	}

	m_timestamps.push_back(input.last_timestamp(EEG));

	LOG(INFO) << "computing the online staging...";

	if (m_first_timestamp == 0) {
		m_first_timestamp = input.last_timestamp(EEG);
	}

	m_last_eeg_index = eeg_sample_index;
	m_last_ir_index = ir_sample_index;

	ullong current_ts = input.last_timestamp(EEG);
	int seconds_since_start = static_cast<int> (current_ts - m_first_timestamp);
	LOG(INFO) << "seconds since start: " << seconds_since_start << ", first ts: " << m_first_timestamp << ", current_ts: " << current_ts;
	dlib::matrix<double> eeg_signal = range_to_dlib_matrix(input.eeg_signal().end() - EEG_WINDOW, input.eeg_signal().end());
	dlib::matrix<double> ir_signal = range_to_dlib_matrix(input.ir_led_signal().end() - IR_WINDOW, input.ir_led_signal().end());

	assert(eeg_signal.nc() == 1);
	assert(ir_signal.nc() == 1);


	m_model.step(eeg_signal, ir_signal, seconds_since_start);
	std::vector<int> staging_from_model = m_model.current_staging();
	SleepStagingResult result(staging_from_model, m_model.current_quality(), m_timestamps);

	LOG(INFO) << "computed! feeding the sinks...";
	feed_all_sinks(result);
}

void OnlineStagingAlgorithm::end_streaming(const INeuroonSignals & input) {
	m_model.stop();
	std::vector<int> staging_from_model = m_model.current_staging();
	SleepStagingResult result(staging_from_model, m_model.current_quality(), m_timestamps);
	feed_all_sinks(result);
}
