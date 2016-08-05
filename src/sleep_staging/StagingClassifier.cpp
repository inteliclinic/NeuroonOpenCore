/*
 * SleepStagingClassifier.cpp
 *
 *  Created on: Aug 5, 2016
 *      Author: tomek
 */

#include "StagingClassifier.h"
#include "MlpClassifier.h"
#include <vector>
#include <fstream>
#include <istream>
#include <sstream>

StagingClassifier* StagingClassifier::m_instance = nullptr;

StagingClassifier* StagingClassifier::get_instance() {
	if (m_instance == nullptr) {
		m_instance = new StagingClassifier();
	}
	return m_instance;
}

dlib::matrix<double> StagingClassifier::load_matrix(const std::string& filename) {
	std::ifstream input(filename);
	if(!input.is_open()) {
		std::stringstream ss;
		ss << "Cannot open file: " << filename;
		throw std::logic_error(ss.str());
	}

	return load_matrix(input);
}

dlib::matrix<double> StagingClassifier::load_matrix(std::istream &input) {
	dlib::matrix<double> result;
	input >> result;
	return result;
}

StagingClassifier::StagingClassifier()
: m_mlp(nullptr)
{
	std::vector<dlib::matrix<double>> weights(2);
	std::vector<dlib::matrix<double>> intercepts(2);

	std::string W1_MATRIX_FILENAME("./res/model/w1.csv");
	std::string W2_MATRIX_FILENAME("./res/model/w2.csv");
	std::string I1_MATRIX_FILENAME("./res/model/i1.csv");
	std::string I2_MATRIX_FILENAME("./res/model/i2.csv");

	weights[0] = load_matrix(W1_MATRIX_FILENAME);
	weights[1] = load_matrix(W2_MATRIX_FILENAME);

	intercepts[0] = load_matrix(I1_MATRIX_FILENAME);
	intercepts[1] = load_matrix(I2_MATRIX_FILENAME);

	m_mlp = new MlpClassifier(weights, intercepts);
}

StagingClassifier::~StagingClassifier() {
	// TODO Auto-generated destructor stub
}


dlib::matrix<int> StagingClassifier::predict(const dlib::matrix<double> &input) {
	return m_mlp->predict(input);
}
