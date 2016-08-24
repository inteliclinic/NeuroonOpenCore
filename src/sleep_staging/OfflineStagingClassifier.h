/*
 * SleepStagingClassifier.h
 *
 *  Created on: Aug 5, 2016
 *      Author: tomek
 */

#ifndef SRC_SLEEP_STAGING_OFFLINESTAGINGCLASSIFIER_H_
#define SRC_SLEEP_STAGING_OFFLINESTAGINGCLASSIFIER_H_

#include <dlib/matrix.h>

class MlpClassifier;

class OfflineStagingClassifier {
	OfflineStagingClassifier();
	static OfflineStagingClassifier* m_instance;

	MlpClassifier* m_mlp;
public:

	static OfflineStagingClassifier* get_instance();
	virtual ~OfflineStagingClassifier();

	virtual dlib::matrix<int> predict(const dlib::matrix<double> &input);
};

#endif /* SRC_SLEEP_STAGING_OFFLINESTAGINGCLASSIFIER_H_ */
