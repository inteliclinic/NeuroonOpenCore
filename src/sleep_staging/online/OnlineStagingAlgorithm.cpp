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
	//LOG(WARNING) << "GOT IT!!!!!   eeg:" << input.eeg_signal().size()<< ", ir: " << input.ir_led_signal().size() <<", total samples: " << input.total_signal_samples(EEG);

	ullong eeg_sample_index = input.total_signal_samples(EEG);
	ullong ir_sample_index = input.total_signal_samples(IR_LED);

	if (m_last_eeg_index + EEG_INTERVAL > eeg_sample_index
		&& m_last_ir_index + IR_INTERVAL > ir_sample_index) {

		//nothing to be done yet -- not enough data
		return;
	}

	if (input.eeg_signal().size() < EEG_WINDOW || input.ir_led_signal().size() < IR_WINDOW) {
		LOG(INFO) << "eeg_samples: " << input.eeg_signal().size() << ", ir samples: " << input.ir_led_signal().size();
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

	dlib::matrix<double> eeg_signal = range_to_dlib_matrix(input.eeg_signal().end() - EEG_WINDOW, input.eeg_signal().end());
	dlib::matrix<double> ir_signal = range_to_dlib_matrix(input.ir_led_signal().end() - IR_WINDOW, input.ir_led_signal().end());

	assert(eeg_signal.nc() == 1);
	assert(ir_signal.nc() == 1);


	std::vector<int> staging_from_model = m_model.step(eeg_signal, ir_signal, seconds_since_start);
	SleepStagingResult result(staging_from_model, m_timestamps);

	LOG(INFO) << "computed! feeding the sinks...";
	feed_all_sinks(result);
}

void OnlineStagingAlgorithm::end_streaming(const INeuroonSignals & input) {
	std::vector<int> staging_from_model = m_model.stop();
	SleepStagingResult result(staging_from_model, m_timestamps);
	feed_all_sinks(result);
}
