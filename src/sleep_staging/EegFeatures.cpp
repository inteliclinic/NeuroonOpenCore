/*
 * EegFeatures.cpp
 *
 *  Created on: Jul 28, 2016
 *      Author: tomek
 */

#include "EegFeatures.h"
#include "Spectrogram.h"
#include <cmath>
#include <algorithm>

EegFeatures::EegFeatures() {
	// TODO Auto-generated constructor stub

}

EegFeatures::~EegFeatures() {
	// TODO Auto-generated destructor stub
}

dlib::matrix<double> EegFeatures::sum_in_band(const Spectrogram& s, double low, double high) {
	int nrows = s.size();
	int ncols = 1;

	dlib::matrix<double> result = dlib::sum_cols(s.get_band(low, high));

	return result;
}

dlib::matrix<double> EegFeatures::sum_in_bands(const Spectrogram& s, std::vector<std::pair<double, double>> bands) {
	dlib::matrix<double> result(s.size(), bands.size());

	for (std::size_t i = 0; i != bands.size(); ++i) {
		dlib::set_colm(result, i) = sum_in_band(s, bands[i].first, bands[i].second);
	}

	return result;
}


dlib::matrix<int> nonnan_rows(const dlib::matrix<double> input) {
	std::vector<int> v;

	for (std::size_t i = 0; i != input.nr(); ++i) {
		bool has_nan = dlib::is_finite(dlib::rowm(input, i));
		if (has_nan) {
			v.push_back(i);
		}
	}

	dlib::matrix<int> result(v.size(), 1);
	for (std::size_t i = 0; i != v.size(); ++i) {
		result(i,0) = v[i];
	}
	return result;
}

//centered, inserts NaNs in the beginning and in the end
dlib::matrix<double> EegFeatures::rolling_mean(const dlib::matrix<double> &signal, int window_size) {
	dlib::matrix<double> result(signal.nr(), 1);
	dlib::set_all_elements(result, NAN);
	for (size_t i = 0; i != signal.nr() - window_size; ++i) {
		dlib::matrix<double> window = dlib::rowm(signal, dlib::range(i, i+window_size-1));

		double sum = dlib::sum(window);
		double mean = sum / window_size;
		result(i + (window_size/2), 0) = mean;
	}
	return result;
}

//doesn't take NaNs into consideration, takes neighbouring values instead
dlib::matrix<double> EegFeatures::sparse_rolling_mean(const dlib::matrix<double> &signal, int window_size) {
	dlib::matrix<double> index(signal.nr(), 1);
	dlib::set_colm(index, 0) = dlib::trans(dlib::range(0, signal.nr() - 1));
	dlib::matrix<int> nonnans = nonnan_rows(signal);

	dlib::matrix<double> rolling_mean_input = dlib::rowm(signal, nonnans);

	dlib::matrix<double> result(signal.nr(), signal.nc());
	dlib::set_all_elements(result, NAN);
	dlib::set_rowm(result, nonnans) = rolling_mean(rolling_mean_input, window_size);

	return result;
}


dlib::matrix<double> EegFeatures::n_max_to_median(const dlib::matrix<double> &data, int n) {
	dlib::matrix<double> input = data;
	dlib::matrix<double> result(data.nr(), 1);

	for (std::size_t i = 0; i != data.nr(); ++i) {
		dlib::matrix<double> row = dlib::rowm(input, i);
		std::sort(row.begin(), row.end());

		double median = row(row.nr() / 2, 0);
		double n_max_sum = dlib::sum(dlib::colm(row, dlib::range(row.nc() - n, row.nc() - 1)));
		result(i, 0) = n_max_sum / median;
	}

	return result;
}
