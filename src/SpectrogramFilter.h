/*
 * SpectrogramFilter.h
 *
 *  Created on: Jul 8, 2016
 *      Author: tomek
 */

#ifndef SRC_SPECTROGRAMFILTER_H_
#define SRC_SPECTROGRAMFILTER_H_

class Spectrogram;

class SpectrogramFilter {
public:
	SpectrogramFilter();

	virtual void filter(Spectrogram& s);
	virtual ~SpectrogramFilter();

	const double INCORRECT = 0;
};

#endif /* SRC_SPECTROGRAMFILTER_H_ */
