/*
 * OnlineStagingClassifier.cpp
 *
 *  Created on: Aug 23, 2016
 *      Author: tomek
 */

#include "OnlineStagingClassifier.h"
#include <vector>
#include "dlib_utils.h"
#include "MlpClassifier.h"
#include "OnLineViterbiSearch.h"

OnlineStagingClassifier::OnlineStagingClassifier() {

	dlib::matrix<int> c = dlib::matrix_cast<int>(load_matrix(ONLINE_MODEL_RES_DIRECTORY "classes.csv"));
	m_classes = dlib_matrix_to_vector(c);

	initialize_mlp();
	initialize_viterbi(m_classes);
}

void OnlineStagingClassifier::initialize_mlp() {
	std::vector<dlib::matrix<double>> weights(2);
	std::vector<dlib::matrix<double>> intercepts(2);

	std::string W1_MATRIX_FILENAME(ONLINE_MODEL_RES_DIRECTORY "/w1.csv");
	std::string W2_MATRIX_FILENAME(ONLINE_MODEL_RES_DIRECTORY "/w2.csv");
	std::string I1_MATRIX_FILENAME(ONLINE_MODEL_RES_DIRECTORY "/i1.csv");
	std::string I2_MATRIX_FILENAME(ONLINE_MODEL_RES_DIRECTORY "/i2.csv");

	weights[0] = load_matrix(W1_MATRIX_FILENAME);
	weights[1] = load_matrix(W2_MATRIX_FILENAME);

	intercepts[0] = load_matrix(I1_MATRIX_FILENAME);
	intercepts[1] = load_matrix(I2_MATRIX_FILENAME);

	m_mlp = new MlpClassifier(weights, intercepts);
}

void OnlineStagingClassifier::initialize_viterbi(const std::vector<int> classes) {
	dlib::matrix<double> start_p(classes.size(), 1);
	dlib::set_all_elements(start_p, 0);
	start_p(1, 0) = 1;

	dlib::matrix<double> final_p(classes.size(), 1);
	dlib::set_all_elements(final_p, 0);
	final_p(1, 0) = 1;

	dlib::matrix<double> transition_matrix = load_matrix(ONLINE_MODEL_RES_DIRECTORY "transitions.csv");

	m_viterbi = new OnLineViterbiSearch(classes, start_p, final_p, transition_matrix);
}

std::vector<int> OnlineStagingClassifier::predict(const dlib::matrix<double> &features) {
	dlib::matrix<double> probabilities = m_mlp->predict_proba(features);
	m_viterbi->step(probabilities);
	return m_viterbi->best_sequence();
}


OnlineStagingClassifier::~OnlineStagingClassifier() {
	delete m_mlp;
	delete m_viterbi;
}

std::vector<int> OnlineStagingClassifier::stop() {
	m_viterbi->stop();
	return m_viterbi->best_sequence();
}

void OnlineStagingClassifier::reset() {
	delete m_viterbi;
	initialize_viterbi(m_classes);
}
