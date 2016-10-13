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

/**
 * Represents a spectrogram i.e. a series of Fast Fourier Transforms computed
 * for consecutive ranges of time points of a given signal (windows).
 *
 * This implementation is designed to be as consistent with the one provided
 * by the Python's scipy.signal module as possible.
 *
 * Only 'boxcar' (rectangular) windows are now supported because of they're
 * relative simplicity
 */
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
    
    /**
     * The constructor. Computes the spectrogram of the signal 'signal'
     *
     * @param signal : the signal for which the spectrogram is to be 
     * computed
     *
     * @param sampling_frequency : sampling rate of the signal in Hz
     * @param window : the number of samples in the window, doesn't have 
     to be a power of two
     * @param noverlap : size of the overlap between the windows,
     positive smaller than the size of the window
    */
	Spectrogram(const dlib::matrix<double>& signal, double sampling_frequency,
			int window, int noverlap=0);

	virtual ~Spectrogram();

    /**
     * returns a reference to an element at time-index t and frequency-index f
     */ 
	double& at(int t, int f);

    /**
     * Number of time-steps of this spectrogram
     */
	std::size_t size() const {
		return timestamps.size();
	}

    /**
     * Returns the vector of frequencies (the y-axis) of the spectrogram
     */
	const dlib::matrix<double>& get_frequencies() const {
		return frequencies;
	}

    /**
     * Returns the vector of time-points (the x-axis) of thespectrogram
     */
	const dlib::matrix<double>& get_timestamps() const {
		return timestamps;
	}

    /**
     * Returns the underlying matrix containing all of the spectrograms data
     */
	const dlib::matrix<double>& data() const {
		return buffer;
	}

	std::pair<double, double> freq_indices(double low, double high) const;

    /**
     * returns a matrix with a frequency-band of the spectrogram,
     * the band is specified by its lowest and highest frequency
     */
	dlib::matrix<double> get_band(double low, double high) const;

    /**
     * Creates another spectrogram from a frequency band of this spectrogram
     */
	Spectrogram create_from_band(double low, double high) const;

	dlib::matrix<double> get_freq_band(double low, double high) const;

    /**
     * Computes a moment of the spectrogram. This value can be expressed with
     * an equation: M_d = sum(A_i^d * f_i) / sum(A_i ^d)
     *
     * @param degree: the degree of the moment, the higher the degree
     * the more important are the biggest values. The degree 1 yields a simple
     * average. An infinite degree would correspond to returning the biggest 
     * frequency
     */
	dlib::matrix<double> compute_moment(int degree) const;

    /**
     * Same as the above version, but this variation computes several moments
     * with the degrees specified by the degrees parameter
     */ 
	dlib::matrix<double> compute_moment(const std::vector<int>& degrees) const;


    /**
     * Prints the spectrogram to the output stream 'out'
     */
	void print(std::ostream& out) const;
};

#endif /* SRC_SPECTROGRAM_H_ */
