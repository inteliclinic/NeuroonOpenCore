/*
 * EegFeatures.h
 *
 *  Created on: Jul 28, 2016
 *      Author: tomek
 */

#ifndef SRC_SLEEP_STAGING_EEGFEATURES_H_
#define SRC_SLEEP_STAGING_EEGFEATURES_H_

#include <dlib/matrix.h>

class Spectrogram;

class EegFeatures {
public:
	EegFeatures();
	virtual ~EegFeatures();

	static dlib::matrix<double> sum_in_band(const Spectrogram& s, double low, double high);
	static dlib::matrix<double> sum_in_bands(const Spectrogram& s, std::vector<std::pair<double, double>> bands);
	static dlib::matrix<double> sparse_rolling_mean(const dlib::matrix<double> &signal, int window_size);
	static dlib::matrix<double> rolling_mean(const dlib::matrix<double> &signal, int window_size);
	static dlib::matrix<double> n_max_to_median(const dlib::matrix<double> &data, int n);

};

#endif /* SRC_SLEEP_STAGING_EEGFEATURES_H_ */
