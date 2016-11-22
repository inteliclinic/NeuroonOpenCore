/*
 * BrainWaveLevels.cpp
 *
 *  Created on: Sep 26, 2016
 *      Author: tomek
 */

#include "BrainWaveLevels.h"
#include "Features.h"
#include "NeuroonSignalStreamApi.h"
#include <stdexcept>
#include <utility>
BrainWaveLevels::BrainWaveLevels()
: m_smoother(4, 4)
{}

BrainWaveLevels::~BrainWaveLevels() {}

std::vector<ncBrainWaveLevels> BrainWaveLevels::predict(const Spectrogram &spectrogram) {

	bool normalized = true;

	std::vector<std::pair<double, double>> bands({
		std::make_pair(0.1, 3),
		std::make_pair(4, 7),
		std::make_pair(8, 13),
		std::make_pair(16, 31)
	});

	dlib::matrix<double> sums = Features::sum_in_bands(spectrogram, bands);
	dlib::matrix<double> sum = dlib::sum_cols(sums);
	std::cout << "sum " << sum << std::endl;
	std::cout << "bands: " << sums << std::endl;
	for (int i = 0; i != sums.nc(); ++i) {
		dlib::set_colm(sums, i) = dlib::pointwise_multiply(dlib::colm(sums, i), 1 / sum);
	}

	std::vector<ncBrainWaveLevels> result;
	for (int i = 0; i != spectrogram.size(); ++i) {
		dlib::matrix<double> row = dlib::rowm(sums, i);
		m_smoother.feed(row);
		row = m_smoother.value();
		std::cout << "after filter: " << row << std::endl;

		ncBrainWaveLevels levels;
		levels.delta = row(0, 0);
		levels.theta = row(0, 1);
		levels.alpha = row(0, 2);
		levels.beta = row(0, 3);
		result.push_back(levels);
	}
	return result;
}
