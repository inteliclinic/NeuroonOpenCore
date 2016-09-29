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

friend class SimpleSpectrogramFilter;

private:
	dlib::matrix<double> buffer;
	dlib::matrix<double> timestamps;
	dlib::matrix<double> frequencies;

	Spectrogram();

protected:

	dlib::matrix<double>& data() {
		return buffer;
	}

public:

	Spectrogram(const dlib::matrix<double>& signal, double sampling_frequency,
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

	std::pair<double, double> freq_indices(double low, double high) const;

	dlib::matrix<double> get_band(double low, double high) const;

	Spectrogram create_from_band(double low, double high) const;

	dlib::matrix<double> get_freq_band(double low, double high) const;

	dlib::matrix<double> compute_moment(int degree) const;

	dlib::matrix<double> compute_moment(const std::vector<int>& degrees) const;



	void print(std::ostream& out) const;
};

#endif /* SRC_SPECTROGRAM_H_ */
