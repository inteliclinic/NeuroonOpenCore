/*
 * Features.h
 *
 *  Created on: Jul 28, 2016
 *      Author: tomek
 */

#ifndef SRC_SLEEP_STAGING_EEGFEATURES_H_
#define SRC_SLEEP_STAGING_EEGFEATURES_H_

#include <dlib/matrix.h>
#include <functional>
#include <vector>
#include <utility>

class Spectrogram;

/**
 * Utility class for computing various features for sleep classification
 * 
 * At the moment aggregates a lot of different functionality. You can
 * certainly consider it as a candidate for refactoring in the future
 */
class Features {

	static std::vector<std::pair<double, double>> create_bands(const std::vector<double>& borders);

public:
	Features();
	virtual ~Features();
    
    /**
     * Computes a sum of amplitudes in a given frequency band for a spectrogram 
     */ 
	static dlib::matrix<double> sum_in_band(const Spectrogram& s, double low, double high, bool normalized = false);

    /**
     * Same as above, but takes a vector of bands and returns the sums of 
     * each band as columns of the resulting matrix.
     */
	static dlib::matrix<double> sum_in_bands(const Spectrogram& s, const std::vector<std::pair<double, double>>& bands, bool normalized = false);

    /**
     * Computes the sum of amplitudes between given borders of the bands.
     *
     * A little less general as sum_in_bands, because you can't specify
     * bands that aren't bordering on each other this way
     */ 
	static dlib::matrix<double> sum_by_borders(const Spectrogram& s, const std::vector<double>& borders, bool normalized = false);

    /**
     * A rolling operation that doesn't look at the NaN values. It just ignores them.
     */
	static dlib::matrix<double> sparse_rolling(const dlib::matrix<double> &signal, int window_size,
											   std::function<dlib::matrix<double> (const dlib::matrix<double>&, int)> rolling_operation);
    
    /**
     * A rolling mean that doesn't look at the NaN values. It just ignores them.
     */
	static dlib::matrix<double> sparse_rolling_mean(const dlib::matrix<double> &signal, int window_size);

   /**
     * A rolling std that doesn't look at the NaN values. It just ignores them.
     */
	static dlib::matrix<double> sparse_rolling_std(const dlib::matrix<double> &signal, int window_size);

	static dlib::matrix<double> rolling_mean(const dlib::matrix<double> &signal, int window_size);
	static dlib::matrix<double> rolling_std(const dlib::matrix<double> &signal, int window_size);

    /**
     * This function computes the ratio of the sum of n biggest values in a row to the median of that row
     * This value is very useful for indiating the spread of some distribution. 
     * Used especially as an indicator of rhythmicity of heart beats of the subject.
     */
	static dlib::matrix<double> n_max_to_median(const dlib::matrix<double> &data, int n);

	static dlib::matrix<double> standardize(const dlib::matrix<double> &data);
	static dlib::matrix<double> standardize_in_window(const dlib::matrix<double> &data, int window_size);
};

#endif /* SRC_SLEEP_STAGING_EEGFEATURES_H_ */
