/*
 * Spectrogram.cpp
 *
 *  Created on: Jul 6, 2016
 *      Author: tomek
 */

#include "Spectrogram.h"

#include <vector>
#include <numeric>

#include <dlib/matrix.h>

Spectrogram::Spectrogram() {

}

Spectrogram::Spectrogram(const dlib::matrix<double>& signal, double sampling_frequency,
			int window, int noverlap) {

	int nrows = (signal.size() - noverlap)/ (window - noverlap);
	int ncols = window / 2;

	buffer = dlib::matrix<double>(nrows, ncols);

	timestamps = dlib::matrix<double>(nrows, 1);
	set_colm(timestamps, 0) = dlib::trans(dlib::range(0, nrows - 1));
	timestamps *= sampling_frequency;

	int n_freqs = window / 2;
	frequencies = dlib::matrix<double> (n_freqs, 1);
	set_colm(frequencies, 0) = dlib::trans(dlib::range(0, n_freqs - 1));
	frequencies *= sampling_frequency / window;

	//std::cout << frequencies;

	for (int i = 0; i != nrows; ++i) {
		int start = i * (window - noverlap);
		int end = start + window - 1;
		dlib::matrix<double> windowed_signal = dlib::rowm(signal, dlib::range(start, end));
		dlib::matrix<std::complex<double>> windowed_complex_signal = dlib::matrix_cast<std::complex<double>> (windowed_signal);
		dlib::matrix<std::complex<double>> fft_res = fft(windowed_complex_signal);
		int last_frequency = (fft_res.size() / 2) - 1;
		fft_res = dlib::rowm(fft_res, dlib::range(0, last_frequency));
		dlib::set_rowm(buffer, i) = dlib::trans(dlib::abs(fft_res));
	}
}

Spectrogram::~Spectrogram() {

}

std::pair<double, double> Spectrogram::freq_indices(double low, double high) const {
	int freq_ind = 0;
	while (freq_ind != frequencies.size()) {
		if (frequencies(freq_ind, 0) >= low) {
			break;
		}
		++freq_ind;
	}
	int low_freq_ind = freq_ind;

	while (freq_ind != frequencies.size()) {
		if (frequencies(freq_ind, 0) >= high) {
			break;
		}
		++freq_ind;
	}
	int high_freq_ind = freq_ind;

	return std::pair<double, double>(low_freq_ind, high_freq_ind);
}

dlib::matrix<double> Spectrogram::get_band(double low, double high) const {
	auto range_indices = freq_indices(low, high);
	return dlib::colm(buffer, dlib::range(range_indices.first, range_indices.second - 1));
}

dlib::matrix<double> Spectrogram::get_freq_band(double low, double high) const {
	auto range_indices = freq_indices(low, high);
	return dlib::rowm(frequencies, dlib::range(range_indices.first, range_indices.second - 1));
}

Spectrogram Spectrogram::create_from_band(double low, double high) const {
	Spectrogram result;

	result.frequencies = this->get_freq_band(low, high);
	result.buffer = this->get_band(low, high);
	result.timestamps = this->timestamps;

	return result;
}

dlib::matrix<double> Spectrogram::compute_moment(int degree) const {
	dlib::matrix<double> moment = dlib::matrix<double> (timestamps.size(), 1);
	for (int i = 0; i != timestamps.size(); ++i) {
		dlib::matrix<double> row = dlib::rowm(buffer, i);
		row = dlib::pow(row, degree);
		double numerator = dlib::dot(row, frequencies);
		double denominator = dlib::sum(row);

		moment(i, 0) = numerator / denominator;
	}
	return moment;
}

dlib::matrix<double> Spectrogram::compute_moment(const std::vector<int>& degrees) const {
	dlib::matrix<double> moments = dlib::matrix<double>(timestamps.size(), degrees.size());
	for (int i = 0; i != degrees.size(); ++i) {
		set_colm(moments, i) = compute_moment(degrees[i]);
	}

	return moments;
}

void Spectrogram::print(std::ostream& out) const {
	out << buffer;
}
