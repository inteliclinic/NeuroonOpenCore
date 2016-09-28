/*
 * OnlineStagingClassifier.cpp
 *
 *  Created on: Aug 23, 2016
 *      Author: tomek
 */

#include "OnlineStagingClassifier.h"
#include <vector>
#include "dlib_utils.h"
#include "logger.h"
#include "MlpClassifier.h"
#include "OnLineViterbiSearch.h"
#include "ModelOnline.h"
#include "ModelOnlineW1.h"
#include "ModelOnlineW2.h"
#include "Config.h"
#include "BrainWaveLevels.h"
#include "EegSignalQuality.h"

OnlineStagingClassifier::OnlineStagingClassifier() {

	dlib::matrix<int> c = online_model::Classes;
	m_classes = dlib_matrix_to_vector<int>(dlib::trans(c));

	initialize_mlp();
	initialize_viterbi(m_classes);
}

void OnlineStagingClassifier::initialize_mlp() {
	std::vector<dlib::matrix<double>> weights(2);
	std::vector<dlib::matrix<double>> intercepts(2);

	// std::string W1_MATRIX_FILENAME(ONLINE_MODEL_RES_DIRECTORY "/w1.csv");
	// std::string W2_MATRIX_FILENAME(ONLINE_MODEL_RES_DIRECTORY "/w2.csv");
	// std::string I1_MATRIX_FILENAME(ONLINE_MODEL_RES_DIRECTORY "/i1.csv");
	// std::string I2_MATRIX_FILENAME(ONLINE_MODEL_RES_DIRECTORY "/i2.csv");

	weights[0] = online_model::W1;
	weights[1] = online_model::W2;

	intercepts[0] = online_model::I1;
	intercepts[1] = online_model::I2;

	m_mlp = new MlpClassifier(weights, intercepts);
}

void OnlineStagingClassifier::initialize_viterbi(const std::vector<int> classes) {
	dlib::matrix<double> start_p(classes.size(), 1);
	dlib::set_all_elements(start_p, 0);
	start_p(3, 0) = 1;

	dlib::matrix<double> final_p(classes.size(), 1);
	dlib::set_all_elements(final_p, 0);
	final_p(3, 0) = 1;

	// dlib::matrix<double> transition_matrix = load_matrix(ONLINE_MODEL_RES_DIRECTORY "/transitions.csv");
	dlib::matrix<double> transition_matrix = online_model::Transitions;
  // LOG(INFO) << transition_matrix;
	assert(transition_matrix.nc() != 0);
	assert(transition_matrix.nr() != 0);

	const double VITERBI_WEIGHT = 1;
	m_viterbi = new OnLineViterbiSearch(classes, start_p, final_p, transition_matrix, VITERBI_WEIGHT);
}

std::vector<int> OnlineStagingClassifier::predict(const dlib::matrix<double> &features) {
	dlib::matrix<double> probabilities;
	LOG(INFO) << "features: " << features;
	if (dlib::is_finite(features)) {
		probabilities = m_mlp->predict_proba(features);
		LOG(INFO) << "probabilities from mlp: " << probabilities;
	} else {
		bool beginning = features(0,5) == 1;
		probabilities = get_probability_when_nan(beginning);
	}
	m_viterbi->step(dlib::trans(probabilities));
	auto result = m_viterbi->best_sequence();
	LOG(INFO) << "staging length: " << result.size();
	return result;
}

dlib::matrix<double> OnlineStagingClassifier::get_probability_when_nan(bool beginning) {
    static std::vector<double> bad_signal_p({0.1, 0.29, 0.30, 0.31});
    static std::vector<double> bad_signal_p_beginning({0.15, 0.15, 0.15, 0.55});


    if (beginning) {
    	return dlib::trans(vector_to_dlib_matrix<double>(bad_signal_p_beginning));
    } else {
    	return dlib::trans(vector_to_dlib_matrix<double>(bad_signal_p));
    }
}

OnlineStagingClassifier::~OnlineStagingClassifier() {
	delete m_mlp;
	delete m_viterbi;
}

void OnlineStagingClassifier::stop() {
	m_viterbi->stop();
	m_current_staging = m_viterbi->best_sequence();
}

void OnlineStagingClassifier::reset() {
	delete m_viterbi;
	m_current_quality.clear();
	m_current_staging.clear();
	initialize_viterbi(m_classes);
}

void OnlineStagingClassifier::compute_staging(const Spectrogram& eeg_spectrogram, const Spectrogram& ir_spectrogram,
											  double seconds_since_start) {

	auto preprocessed = m_preprocessor.transform(eeg_spectrogram, ir_spectrogram, seconds_since_start);
	m_current_staging = predict(preprocessed.features);
}

void OnlineStagingClassifier::compute_quality(const Spectrogram& eeg_spectrogram) {
	EegSignalQuality quality_computer;
	int quality = quality_computer.predict(eeg_spectrogram);
	m_current_quality.push_back(quality);
}

void OnlineStagingClassifier::compute_brain_waves(const Spectrogram& eeg_spectrogram) {
	BrainWaveLevels m_bw;
	brain_wave_levels_t levels = m_bw.predict(eeg_spectrogram).front();
	m_current_brain_waves.push_back(levels);
}

void OnlineStagingClassifier::step(const dlib::matrix<double> eeg_signal,
											  const dlib::matrix<double> ir_signal,
											  double seconds_since_start) {

	const int overlap = 0;
	const int EEG_FFT_WINDOW = 10 * 1024;
	//const int EEG_FFT_OVERLAP = (EEG_FFT_WINDOW * 3) / 4;
	const int IR_FFT_WINDOW = 2048;
	//const int IR_FFT_OVERLAP = (2048 *3) / 4;

	Spectrogram eeg_spectrogram(eeg_signal, Config::instance().neuroon_eeg_freq(), EEG_FFT_WINDOW, overlap);
	Spectrogram ir_spectrogram(ir_signal, Config::instance().neuroon_ir_freq(), IR_FFT_WINDOW, overlap);

	compute_quality(eeg_spectrogram);
	compute_staging(eeg_spectrogram, ir_spectrogram, seconds_since_start);
	compute_brain_waves(eeg_spectrogram);
}

const std::vector<int>& OnlineStagingClassifier::current_staging() const {
	return m_current_staging;
}

const std::vector<int>& OnlineStagingClassifier::current_quality() const {
	return m_current_quality;
}

const std::vector<brain_wave_levels_t>& OnlineStagingClassifier::current_brain_waves() const {
	return m_current_brain_waves;
}
