/*
 * MlpClassifier.cpp
 *
 *  Created on: Aug 5, 2016
 *      Author: tomek
 */

#include "MlpClassifier.h"

#include "dlib_utils.h"

MlpClassifier::MlpClassifier(std::vector<dlib::matrix<double>> weights, std::vector<dlib::matrix<double>> intercepts)
: m_mlp(weights, intercepts)
{}

MlpClassifier::~MlpClassifier() {}

dlib::matrix<int> MlpClassifier::predict(const dlib::matrix<double>& input) {
	dlib::matrix<double> mlp_output = m_mlp.predict(input);
	std::cout << "stages nan ratio: " << nan_ratio(mlp_output) << std::endl;
	dlib::matrix<int> classes_output = argmax(mlp_output);
	return classes_output;
}

dlib::matrix<double> MlpClassifier::predict_proba(const dlib::matrix<double>& input) {
	dlib::matrix<double> mlp_output = m_mlp.predict(input);
	dlib::matrix<double> probabilities = softmax(mlp_output);
	return probabilities;
}
