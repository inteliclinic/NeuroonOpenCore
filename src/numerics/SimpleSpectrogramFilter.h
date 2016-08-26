/*
 * SpectrogramFilter.h
 *
 *  Created on: Jul 8, 2016
 *      Author: tomek
 */

#ifndef SRC_SIMPLESPECTROGRAMFILTER_H_
#define SRC_SIMPLESPECTROGRAMFILTER_H_

class Spectrogram;

class SimpleSpectrogramFilter {
public:
	SimpleSpectrogramFilter();

	virtual void filter(Spectrogram& s);
	virtual ~SimpleSpectrogramFilter();

	const double INCORRECT = 0;
};

#endif /* SRC_SIMPLESPECTROGRAMFILTER_H_ */
