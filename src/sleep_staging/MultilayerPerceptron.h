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

public:

	MultilayerPerceptron(std::vector<dlib::matrix<double>> weights, std::vector<dlib::matrix<double>> intercepts);
	virtual ~MultilayerPerceptron();
	dlib::matrix<double> predict(const dlib::matrix<double>& input);

};

#endif /* SRC_SLEEP_STAGING_MULTILAYERPERCEPTRON_H_ */
