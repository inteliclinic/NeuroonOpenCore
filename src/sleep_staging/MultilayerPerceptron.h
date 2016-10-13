/*
 * MultiLayerPerceptron.h
 *
 *  Created on: Aug 4, 2016
 *      Author: tomek
 */

#ifndef SRC_SLEEP_STAGING_MULTILAYERPERCEPTRON_H_
#define SRC_SLEEP_STAGING_MULTILAYERPERCEPTRON_H_

#include <vector>
#include <dlib/matrix.h>


/**
 * This class implements a multilayer perceptron, i.e. a relatively simple 
 * artificial neural network.
 * 
 * MLP can be used as a 'backend' for classification and regression tasks
 */
class MultilayerPerceptron {

	struct ActivationFunction {
		virtual dlib::matrix<double> operator()(const dlib::matrix<double> &input) const = 0;
		virtual ~ActivationFunction() {}
	};

	struct RectifiedLinearActivationFunction : public ActivationFunction {
		virtual dlib::matrix<double> operator()(const dlib::matrix<double> &input) const override ;
	};

	struct LinearActivationFunction : public ActivationFunction {
		virtual dlib::matrix<double> operator()(const dlib::matrix<double> &input) const override;
	};

	std::vector<dlib::matrix<double>> m_weights;
	std::vector<dlib::matrix<double>> m_intercepts;
	std::vector<ActivationFunction*> m_activations;

	bool check_matrices_dimensions();

public:

	MultilayerPerceptron(std::vector<dlib::matrix<double>> weights, std::vector<dlib::matrix<double>> intercepts);
	virtual ~MultilayerPerceptron();

    /**
     * Return the outputs of the MLP for given inputs
     * @param input : the values of inputs for the network
     *        each row corresponding to one observation,
     *        each column to one feature.
     * @return a dlib matrix containing the outputs of the network,
     */
	dlib::matrix<double> predict(const dlib::matrix<double>& input);

};

#endif /* SRC_SLEEP_STAGING_MULTILAYERPERCEPTRON_H_ */
