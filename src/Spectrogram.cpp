/*
 * Spectrogram.cpp
 *
 *  Created on: Jul 6, 2016
 *      Author: tomek
 */

#include "Spectrogram.h"

#include <vector>
#include <algorithm>

#include "fft.h"

Spectrogram::Spectrogram(const std::vector<double>& signal, int sampling_frequency,
			int window, int noverlap) {

	int spec_length = signal.size() / (window - noverlap);

	buffer = std::vector<std::vector<double>>(spec_length);

	timestamps = std::vector<double> (signal.size() / (window - noverlap));
	std::iota(timestamps.begin(), timestamps.end(), 0);
	for (auto& t : timestamps) {
		t = t * (sampling_frequency / window);
	}

	frequencies = std::vector<double>(window);
	std::iota(frequencies.begin(), frequencies.end(), 0);
	for (auto& t : timestamps) {
		t = t / sampling_frequency;
	}

	for (int i = 0; i != spec_length; ++i) {
		int start = i * (window - noverlap);
		int end = start + window;
		std::vector<double> fft_res = fft(signal, start, end);
		buffer[i] = fft_res;
	}
}

double& Spectrogram::at(int t, int f) {
	return buffer[t][f];
}

Spectrogram::~Spectrogram() {
	// TODO Auto-generated destructor stub
}

void Spectrogram::print(std::ostream& out) {
	for (std::size_t i = 0; i != timestamps.size(); ++i) {
		for (std::size_t j = 0; j != frequencies.size(); ++j) {
			out << buffer[i][j];
			if (j != frequencies.size() - 1) {
				out << ',';
			}
		}
		out << std::endl;
	}
}
