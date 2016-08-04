/*
 * MultiLayerPerceptron.cpp
 *
 *  Created on: Aug 4, 2016
 *      Author: tomek
 */

#include "MultilayerPerceptron.h"

MultilayerPerceptron::MultilayerPerceptron(std::vector<dlib::matrix<double>> weights, std::vector<dlib::matrix<double>> intercepts)
: m_weights(weights)
,  m_intercepts(intercepts)
{
	m_activations.push_back(new RectifiedLinearActivationFunction());
	m_activations.push_back(new LinearActivationFunction());
}

MultilayerPerceptron::~MultilayerPerceptron() {
	for (auto a : m_activations) {
		delete a;
	}
}

dlib::matrix<double> MultilayerPerceptron::RectifiedLinearActivationFunction::operator()(const dlib::matrix<double> &input) const {
	dlib::matrix<double> result;
	result = input;
	for (int i = 0; i != result.nr(); ++i) {
		for (int j = 0; j != result.nc(); ++j) {
			if (result(i,j) < 0) {
				result(i,j) = 0;
			}
		}
	}
	return result;
}

dlib::matrix<double> MultilayerPerceptron::LinearActivationFunction::operator()(const dlib::matrix<double> &input) const {
	dlib::matrix<double> result = input;
	return result;
}

dlib::matrix<double> MultilayerPerceptron::predict(const dlib::matrix<double>& input) {
	dlib::matrix<double> current_layer_output = input;
	for (int i = 0; i != m_weights.size(); ++i) {
		current_layer_output = current_layer_output * m_weights[i];

		for (int j = 0; j != current_layer_output.nr(); ++j) {
			dlib::set_rowm(current_layer_output, j) = dlib::rowm(current_layer_output, j) + m_intercepts[i];
		}

		current_layer_output = m_activations[i]->operator()(current_layer_output);
	}

	return current_layer_output;
}
