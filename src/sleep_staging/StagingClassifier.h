/*
 * SleepStagingClassifier.h
 *
 *  Created on: Aug 5, 2016
 *      Author: tomek
 */

#ifndef SRC_SLEEP_STAGING_STAGINGCLASSIFIER_H_
#define SRC_SLEEP_STAGING_STAGINGCLASSIFIER_H_

#include <dlib/matrix.h>

class MlpClassifier;

class StagingClassifier {
	StagingClassifier();
	static StagingClassifier* m_instance;

	MlpClassifier* m_mlp;
public:

	static StagingClassifier* get_instance();
	virtual ~StagingClassifier();

	virtual dlib::matrix<int> predict(const dlib::matrix<double> &input);
};

#endif /* SRC_SLEEP_STAGING_STAGINGCLASSIFIER_H_ */
