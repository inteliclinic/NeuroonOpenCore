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

class MlpClassifier;
class OnLineViterbiSearch;

class OnlineStagingClassifier {

	MlpClassifier* m_mlp = nullptr;
	OnLineViterbiSearch* m_viterbi = nullptr;

	OnlineStagingFeaturePreprocessor m_preprocessor;
	std::vector<int> m_classes;

	void initialize_mlp();
	void initialize_viterbi(const std::vector<int> classes);

	dlib::matrix<double> get_probability_when_nan(bool beginning);

public:
	OnlineStagingClassifier();
	~OnlineStagingClassifier();

	std::vector<int> predict(const dlib::matrix<double> &features);

	std::vector<int> step(const dlib::matrix<double> eeg_signal,
						  const dlib::matrix<double> ir_signal,
						  double seconds_since_start);
	std::vector<int> stop();
	void reset();
};

#endif /* SRC_SLEEP_STAGING_ONLINESTAGINGCLASSIFIER_H_ */
