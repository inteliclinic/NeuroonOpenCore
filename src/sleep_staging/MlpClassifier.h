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

class MlpClassifier {
private:
	MultilayerPerceptron m_mlp;

public:


	MlpClassifier(std::vector<dlib::matrix<double>> weights, std::vector<dlib::matrix<double>> intercepts);
	virtual ~MlpClassifier();

	dlib::matrix<int> predict(const dlib::matrix<double>& input);
};

#endif /* SRC_SLEEP_STAGING_MLPCLASSIFIER_H_ */
