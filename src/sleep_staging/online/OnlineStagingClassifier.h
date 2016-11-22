/*
 * OnlineStagingClassifier.h
 *
 *  Created on: Aug 23, 2016
 *      Author: tomek
 */

#ifndef SRC_SLEEP_STAGING_ONLINESTAGINGCLASSIFIER_H_
#define SRC_SLEEP_STAGING_ONLINESTAGINGCLASSIFIER_H_
#include <vector>

#include "OnlineStagingFeaturePreprocessor.h"
#include "NeuroonSignalStreamApi.h"
#include "BrainWaveLevels.h"

class MlpClassifier;
class OnLineViterbiSearch;

/**
 * Aggregates all tasks and logic for online processing of sleep
 * i.e.: sleep staging, computing quality, brain wave levels etc.
 * also computes the necessary spectrograms. This aggregation was developed
 * to ensure the most CPU-intensive tasks (spectrograms) aren't
 * being carried out repeatedly
 */
class OnlineStagingClassifier {

	MlpClassifier* m_mlp = nullptr;
	OnLineViterbiSearch* m_viterbi = nullptr;
	BrainWaveLevels m_bw;

	OnlineStagingFeaturePreprocessor m_preprocessor;
	std::vector<int> m_classes;

	void initialize_mlp();
	void initialize_viterbi(const std::vector<int> classes);

	dlib::matrix<double> get_probability_when_nan(bool beginning);

	std::vector<int> m_current_staging;
	std::vector<int> m_current_quality;
	std::vector<ncBrainWaveLevels> m_current_brain_waves;

	void compute_quality(const Spectrogram& eeg_spectrogram);
	void compute_staging(const Spectrogram& eeg_spectrogram, const Spectrogram &ir_spectrogram, double seconds_since_start);
	void compute_brain_waves(const Spectrogram& eeg_spectrogram);


public:
	OnlineStagingClassifier();
	~OnlineStagingClassifier();

	std::vector<int> predict(const dlib::matrix<double> &features);

	void step(const dlib::matrix<double> eeg_signal,
						  const dlib::matrix<double> ir_signal,
						  double seconds_since_start);
	void stop();
	void reset();

	const std::vector<int>& current_staging() const;
	const std::vector<int>& current_quality() const;
	const std::vector<ncBrainWaveLevels>& current_brain_waves() const;

};

#endif /* SRC_SLEEP_STAGING_ONLINESTAGINGCLASSIFIER_H_ */
