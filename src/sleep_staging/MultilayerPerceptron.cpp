/*
 * MultiLayerPerceptron.cpp
 *
 *  Created on: Aug 4, 2016
 *      Author: tomek
 */

#include "MultilayerPerceptron.h"
#include <sstream>

MultilayerPerceptron::MultilayerPerceptron(std::vector<dlib::matrix<double>> weights, std::vector<dlib::matrix<double>> intercepts)
: m_weights(weights)
,  m_intercepts(intercepts)
{
	//CURRENTLY ONLY TWO HIDDEN LAYERS ARE SUPPORTED BUT THIS CAN BE EASILY CHANGED
	m_activations.push_back(new RectifiedLinearActivationFunction());
	m_activations.push_back(new LinearActivationFunction());

	check_matrices_dimensions();
}

bool MultilayerPerceptron::check_matrices_dimensions() {
	if (m_weights.size() != m_intercepts.size()) {
		throw std::logic_error("weights and intercepts vector sizes don't agree ");
	}

	if (m_weights.size() != m_activations.size()) {
		throw std::logic_error("weights and activations vector sizes don't agree");
	}

	for (size_t i = 0; i != m_weights.size() -1; ++i) {
		if (m_weights[i].nc() != m_weights[i+1].nr()) {
			std::stringstream ss;
			ss << "The dimensions of the matrices in layers: " << i << " and " << i + 1 << " don't agree: " << std::endl
			   << "w1: " << m_weights[i].nr() << "x" << m_weights[i].nc() << std::endl
			   << "w2: " << m_weights[i+1].nr() << "x" << m_weights[i+1].nc() << std::endl;
			throw std::logic_error(ss.str());
		}
	}

	return true;
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

	if(input.nc() != m_weights[0].nr()) {
		std::stringstream ss;
		ss << "This network has " << m_weights[0].nr() << " input neurons";
		throw std::logic_error(ss.str());
	}

	dlib::matrix<double> current_layer_output = input;
	for (int i = 0; i != m_weights.size(); ++i) {
		current_layer_output = current_layer_output * m_weights[i];

		for (int j = 0; j != current_layer_output.nr(); ++j) {
			dlib::set_rowm(current_layer_output, j) = dlib::rowm(current_layer_output, j) + dlib::trans(m_intercepts[i]);
		}

		current_layer_output = m_activations[i]->operator()(current_layer_output);
	}

	return current_layer_output;
}
