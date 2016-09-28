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
#include "NeuroonAlgCoreApi.h"
#include "BrainWaveLevels.h"

class MlpClassifier;
class OnLineViterbiSearch;

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
	std::vector<brain_wave_levels_t> m_current_brain_waves;

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
	const std::vector<brain_wave_levels_t>& current_brain_waves() const;

};

#endif /* SRC_SLEEP_STAGING_ONLINESTAGINGCLASSIFIER_H_ */
