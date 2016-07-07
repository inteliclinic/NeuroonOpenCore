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
#include <dlib/matrix.h>

class Spectrogram {
private:
	dlib::matrix<double> buffer;
	dlib::matrix<double> timestamps;
	dlib::matrix<double> frequencies;

public:
	Spectrogram(const dlib::matrix<double>& signal, int sampling_frequency,
			int window, int noverlap=0);

	virtual ~Spectrogram();

	double& at(int t, int f);

	std::size_t size() const {
		return timestamps.size();
	}


//	const dlib::matrix<double> & fft_at(int t) const {
//		return dlib::rowm(buffer, t);
//	}

	const dlib::matrix<double>& get_frequencies() const {
		return frequencies;
	}

	const dlib::matrix<double>& get_timestamps() const {
		return timestamps;
	}

	const dlib::matrix<double>& data() const {
		return buffer;
	}

	void print(std::ostream& out);
};

#endif /* SRC_SPECTROGRAM_H_ */
