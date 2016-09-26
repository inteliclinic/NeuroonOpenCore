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

OnlinePresentationAlgorithm::OnlinePresentationAlgorithm(const std::vector<OnlinePresentationAlgorithm::sink_t*> & sinks)
: SinkStreamingAlgorithm<OnlinePresentationResult>(sinks)
{
	m_last_eeg_index = 0;
}

OnlinePresentationAlgorithm::~OnlinePresentationAlgorithm() {}

void OnlinePresentationAlgorithm::reset_state() {}

void OnlinePresentationAlgorithm::process_input(const INeuroonSignals & input) {
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
	presentation_element_t pe;
	pe.brain_waves = bw.predict(eeg_spectrogram);
	pe.heart_rate = 0;

	const int ELEMENTS_TO_REMEMBER = 256;
	m_buffer.push_back(pe);
	if (m_buffer.size() > ELEMENTS_TO_REMEMBER) {
		m_buffer.erase(m_buffer.begin());
	}

	OnlinePresentationResult result;
	result.data = m_buffer.data();
	result.size = m_buffer.size();
	feed_all_sinks(result);
}

void OnlinePresentationAlgorithm::end_streaming(const INeuroonSignals & input) {
}
