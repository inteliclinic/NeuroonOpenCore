/*
 * SpectrogramFilter.cpp
 *
 *  Created on: Jul 8, 2016
 *      Author: tomek
 */
#include "SimpleSpectrogramFilter.h"

#include <iostream>

#include <dlib/matrix.h>
#include "dlib_utils.h"

#include "Spectrogram.h"


SimpleSpectrogramFilter::SimpleSpectrogramFilter() {
	// TODO Auto-generated constructor stub

}

SimpleSpectrogramFilter::~SimpleSpectrogramFilter() {
	// TODO Auto-generated destructor stub
}

void SimpleSpectrogramFilter::filter(Spectrogram& s) {
	int thresh = percentile(s.data(), 0.50);
	for (int r = 0; r != s.data().nr(); ++r) {
		dlib::matrix<double> row = dlib::rowm(s.data(), r);
		auto mask = row > thresh;

		double over_the_thresh_ratio = dlib::sum(mask) / s.data().nc();
		if (over_the_thresh_ratio > 0.95) {
			dlib::set_rowm(s.data(), r) = INCORRECT;
		}
	}
}
