/*
 * OnlineStagingClassifier.h
 *
 *  Created on: Aug 23, 2016
 *      Author: tomek
 */

#ifndef SRC_SLEEP_STAGING_ONLINESTAGINGCLASSIFIER_H_
#define SRC_SLEEP_STAGING_ONLINESTAGINGCLASSIFIER_H_
#include <vector>
class OnlineStagingClassifier {
public:
	OnlineStagingClassifier();
	~OnlineStagingClassifier();

	void process_input(double NeuroonSignals);
	std::vector<int> get_results();
};

#endif /* SRC_SLEEP_STAGING_ONLINESTAGINGCLASSIFIER_H_ */
