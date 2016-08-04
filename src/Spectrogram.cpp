/*
 * Spectrogram.cpp
 *
 *  Created on: Jul 6, 2016
 *      Author: tomek
 */

#include "Spectrogram.h"

#include <vector>
#include <algorithm>
#include <numeric>
#include <dlib/matrix.h>
#include <exception>


Spectrogram::Spectrogram() {

}


int smaller_power_of_2(int x) {
	int log2 = 0;
	while (x >>= 1) ++log2;

	int result = 1;
	result = result << log2;
	return result;
}

/*
 * currently this is designed and tested to compute the exact C++ equivalent of scipy's:
 * t, f, Sxx = spectrogram(signal, nperseg=window, noverlap=noverlap, mode='magnitude',
 * 						   scaling='spectrum', window=get_window('boxcar'))
 *
 *
 * covering other values of 'mode', 'scaling' and 'window' parameters should be easy, but it requires certain
 * knowledge in signal processing and some time. Since these parameters won't be used for a while, they're
 * currently not implemented
 *
 *
 * WARNING -- this function doesn't compute exact spectrograms for non-power-of-2 window sizes.
 * In those cases it just truncates the window to the closest power of 2 and computes the FFTs with this 'effective_window',
 * the original window is still used for 'moving' the 'effective_window' in subsequent iterations. See the code for details.
 *
 */
Spectrogram::Spectrogram(const dlib::matrix<double>& signal, double sampling_frequency,
			int window, int noverlap) {

	if (signal.size() < noverlap) {
		throw std::logic_error("noverlap greater than signal length");
	}

	int nrows = (signal.size() - noverlap)/ (window - noverlap);
	int effective_window = smaller_power_of_2(window);
	int ncols = effective_window / 2;

	buffer = dlib::matrix<double>(nrows, ncols);

	timestamps = dlib::matrix<double>(nrows, 1);

	//ugly hack that makes it exactly as in scipy's spectrogram
	set_colm(timestamps, 0) = dlib::trans(dlib::range(2, nrows +1));
	timestamps *= (window - noverlap) / sampling_frequency;

	int n_freqs = effective_window / 2;
	frequencies = dlib::matrix<double> (n_freqs, 1);
	set_colm(frequencies, 0) = dlib::trans(dlib::range(0, n_freqs - 1));
	frequencies *= sampling_frequency / effective_window;

	for (int i = 0; i != nrows; ++i) {
		int start = i * (window - noverlap);
		int end = start + effective_window - 1;
		dlib::matrix<double> windowed_signal = dlib::rowm(signal, dlib::range(start, end));
		dlib::matrix<std::complex<double>> windowed_complex_signal = dlib::matrix_cast<std::complex<double>> (windowed_signal);
		dlib::matrix<std::complex<double>> fft_res = fft(windowed_complex_signal);
		int last_frequency = (fft_res.size() / 2) - 1;
		fft_res = dlib::rowm(fft_res, dlib::range(0, last_frequency));
		// TODO: PROBABLY SHOULD BE +1 in order to get fs/2 also.

		bool psd_mode = false;
		bool magnitude_mode = true;
		if (psd_mode) {
		  fft_res = (fft_res / effective_window);
		  fft_res = pointwise_multiply(fft_res, fft_res);//dlib::pow(fft_res, 2);
		  fft_res = 2 * fft_res;
		} else if (magnitude_mode) {
		  fft_res = 2 * fft_res;
		}

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
	if (low > frequencies(frequencies.nr() - 1, 0)) {
		throw std::out_of_range("low band freq too big");
	}

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
	out << dlib::csv << buffer;
}
