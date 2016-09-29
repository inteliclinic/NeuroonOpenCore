/*
 * OnlinePresentationAlgorithm.cpp
 *
 *  Created on: Aug 23, 2016
 *      Author: tomek
 */

#include "OnlinePresentationAlgorithm.h"
#include "Config.h"
#include "MlpClassifier.h"
#include <vector>
#include <dlib/matrix.h>
#include <cassert>
#include "dlib_utils.h"
#include "logger.h"
#include "BrainWaveLevels.h"
#include "Spectrogram.h"
#include "SpectrogramHeartRate.h"
#include <algorithm>

OnlinePresentationAlgorithm::OnlinePresentationAlgorithm(const std::vector<OnlinePresentationAlgorithm::sink_t*> & sinks)
: SinkStreamingAlgorithm<OnlinePresentationResult>(sinks)
{
	m_last_eeg_index = 0;
	m_last_ir_index = 0;
	m_active = false;
}

OnlinePresentationAlgorithm::~OnlinePresentationAlgorithm() {}

void OnlinePresentationAlgorithm::reset_state() {
	m_bw.clear();
	m_pulse_data.clear();
}

void OnlinePresentationAlgorithm::process_input(const INeuroonSignals & input) {
	if (!m_active) {
		return;
	}

	process_brain_waves(input);
	process_pulseoximetry(input);
}

void OnlinePresentationAlgorithm::update_heart_rate(const INeuroonSignals & input) {
	const int IR_WINDOW = 128;
	const int OVERLAP = IR_WINDOW - 8;

	if (m_last_ir_index + (IR_WINDOW - OVERLAP) > input.total_signal_samples(SignalOrigin::IR_LED)) {
		return;
	}

	dlib::matrix<double> ir_signal = range_to_dlib_matrix(input.ir_led_signal().end() - IR_WINDOW, input.ir_led_signal().end());
	Spectrogram ir_spectrogram(ir_signal, Config::instance().neuroon_ir_freq(), IR_WINDOW, OVERLAP);

	SpectrogramHeartRate shr;
	double hr = shr.predict(ir_spectrogram).front();
	m_heart_rate = hr;
}

void OnlinePresentationAlgorithm::update_pulseoximeter_data(const INeuroonSignals & input) {
 	std::size_t PULSE_ELEMENTS = 5 * 25;
 	m_pulse_data.resize(PULSE_ELEMENTS);
 	std::fill(m_pulse_data.begin(), m_pulse_data.end(), 0);
 	std::copy(input.ir_led_signal().end() - std::min(PULSE_ELEMENTS, input.ir_led_signal().size()), input.ir_led_signal().end(),
 			  m_pulse_data.begin()
 	);
}

void OnlinePresentationAlgorithm::process_pulseoximetry(const INeuroonSignals & input) {
	update_heart_rate(input);
	update_pulseoximeter_data(input);
	update_results();
}

void OnlinePresentationAlgorithm::process_brain_waves(const INeuroonSignals & input) {
	const int EEG_WINDOW = 256;
	const int OVERLAP = EEG_WINDOW - 8;

	if (input.total_signal_samples(SignalOrigin::EEG) < EEG_WINDOW) {
		return;
	}

	if (m_last_eeg_index + (EEG_WINDOW - OVERLAP) > input.total_signal_samples(SignalOrigin::EEG)) {
		return;
	}

	m_last_eeg_index = input.total_signal_samples(SignalOrigin::EEG);

	dlib::matrix<double> eeg_signal = range_to_dlib_matrix(input.eeg_signal().end() - EEG_WINDOW, input.eeg_signal().end());
	Spectrogram eeg_spectrogram(eeg_signal, Config::instance().neuroon_eeg_freq(), EEG_WINDOW, OVERLAP);

	BrainWaveLevels bw;
	brain_wave_levels_t b = bw.predict(eeg_spectrogram).front();

	const int ELEMENTS_TO_REMEMBER = 256;
	m_bw.push_back(b);
	if (m_bw.size() > ELEMENTS_TO_REMEMBER) {
		m_bw.erase(m_bw.begin());
	}
	update_results();
}

void OnlinePresentationAlgorithm::end_streaming(const INeuroonSignals & input) {}

void OnlinePresentationAlgorithm::activate() {
	m_active = true;
}

void OnlinePresentationAlgorithm::deactivate() {
	m_active = false;
}

void OnlinePresentationAlgorithm::update_results() {
	OnlinePresentationResult result;
	result.brain_waves = m_bw.data();
	result.bw_size = m_bw.size();
	result.heart_rate = m_heart_rate;
	result.pulse_data = m_pulse_data.data();
	result.pd_size = m_pulse_data.size();
	feed_all_sinks(result);
}
