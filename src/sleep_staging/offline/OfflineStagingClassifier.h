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


/**
 * This class was designed to compute the offline staging, i.e. the staging 
 * after the all the data about sleep is already gathered. It can be considered 
 * as deprecated, because very similar functionality is offered by the online
 * staging functionality and keeping both versions of the algorithm up-to-date
 * can be very resource intensive.
 *
 * You can consider removing this class.
 */ 
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
