/*
 * Spectrogram.h
 *
 *  Created on: Jul 6, 2016
 *      Author: tomek
 */

#ifndef SRC_SPECTROGRAM_H_
#define SRC_SPECTROGRAM_H_

#include <vector>
#include <ostream>
class Spectrogram {
private:
	std::vector<std::vector<double>> buffer;
	std::vector<double> timestamps;
	std::vector<double> frequencies;

public:
	Spectrogram(const std::vector<double>& signal, int sampling_frequency,
			int window, int noverlap=0);

	virtual ~Spectrogram();

	double& at(int t, int f);

	std::size_t size() const {
		return timestamps.size();
	}

	const std::vector<double>& fft_at(int t) const {
		return buffer[t];
	}

	const std::vector<double>& get_frequencies() const {
		return frequencies;
	}

	const std::vector<double>& get_timestamps() const {
		return timestamps;
	}

	void print(std::ostream& out);
};

#endif /* SRC_SPECTROGRAM_H_ */
