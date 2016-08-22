/*
 * ExpandingMean.h
 *
 *  Created on: Aug 22, 2016
 *      Author: tomek
 */

#ifndef SRC_EXPANDINGMEAN_H_
#define SRC_EXPANDINGMEAN_H_

class ExpandingMean {
	double m_sum;
	int m_count;
public:
	ExpandingMean();
	void consume(double x);
	double value() const;
};

#endif /* SRC_EXPANDINGMEAN_H_ */
