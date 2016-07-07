/*
 * Spectrogram.cpp
 *
 *  Created on: Jul 6, 2016
 *      Author: tomek
 */

#include "Spectrogram.h"

#include <vector>
#include <algorithm>

#include <dlib/matrix.h>

Spectrogram::Spectrogram(const dlib::matrix<double>& signal, int sampling_frequency,
			int window, int noverlap) {

	int nrows = (signal.size() - noverlap)/ (window - noverlap);
	int ncols = window;

	buffer = dlib::matrix<double>(nrows, ncols);

	timestamps = dlib::matrix<double> (signal.size() / (window - noverlap), 1);
	std::iota(timestamps.begin(), timestamps.end(), 0);
	timestamps *= sampling_frequency;

	frequencies = dlib::matrix<double>(window, 1);
	std::iota(frequencies.begin(), frequencies.end(), 0);
	frequencies *= sampling_frequency / window;

	for (int i = 0; i != nrows; ++i) {
		int start = i * (window - noverlap);
		int end = start + window - 1;
		dlib::matrix<double> windowed_signal = dlib::rowm(signal, dlib::range(start, end));
		dlib::matrix<std::complex<double>> windowed_complex_signal = dlib::matrix_cast<std::complex<double>> (windowed_signal);
		dlib::matrix<std::complex<double>> fft_res = fft(windowed_complex_signal);
		dlib::set_rowm(buffer, i) = dlib::trans(dlib::abs(fft_res));
	}
}

Spectrogram::~Spectrogram() {
	// TODO Auto-generated destructor stub
}

void Spectrogram::print(std::ostream& out) {
	out << buffer;
}
