/*
 * MlpClassifier.cpp
 *
 *  Created on: Aug 5, 2016
 *      Author: tomek
 */

#include "MlpClassifier.h"
#include "signal_utils.h"

MlpClassifier::MlpClassifier(std::vector<dlib::matrix<double>> weights, std::vector<dlib::matrix<double>> intercepts)
: m_mlp(weights, intercepts)
{
	// TODO Auto-generated constructor stub
}

MlpClassifier::~MlpClassifier() {
	// TODO Auto-generated destructor stub
}

dlib::matrix<int> MlpClassifier::predict(const dlib::matrix<double>& input) {
	dlib::matrix<double> mlp_output = m_mlp.predict(input);
	dlib::matrix<int> classes_output = argmax(mlp_output);
	return classes_output;
}

