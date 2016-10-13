/*
 * MlpClassifier.h
 *
 *  Created on: Aug 5, 2016
 *      Author: tomek
 */

#ifndef SRC_SLEEP_STAGING_MLPCLASSIFIER_H_
#define SRC_SLEEP_STAGING_MLPCLASSIFIER_H_

#include <vector>
#include <dlib/matrix.h>
#include "MultilayerPerceptron.h"

/**
 * An Implementation of a classifier based on a multi-layer perceptron.
 * Doesn't implement any learning procedures, just the predictive part
 * -- implementing the learning procedures in C++ would be too difficult
 * you can train the classifier e.g. using the Python's sklearn.neural_network
 * package, then save the weights and load them from a file to use in C++
 */
class MlpClassifier {
private:

	MultilayerPerceptron m_mlp;

public:

	MlpClassifier(std::vector<dlib::matrix<double>> weights, std::vector<dlib::matrix<double>> intercepts);

	virtual ~MlpClassifier();

    /**
     * Implements the prediction
     * 
     * @param input : the features to be used for classification
     *
     * @returns a dlib matrix with indices of the predicted class labels
     */
	dlib::matrix<int> predict(const dlib::matrix<double>& input);

    /**
     * Implements the probabilistic prediction 
     * 
     * @param input : the features to be used for classification
     *
     * @returns a matrix of probabilities of each label
     */
	dlib::matrix<double> predict_proba(const dlib::matrix<double>& input);
};

#endif /* SRC_SLEEP_STAGING_MLPCLASSIFIER_H_ */
