#ifndef SIGNAL_UTILS_H
#define SIGNAL_UTILS_H

#include <dlib/matrix.h>

/**
 * This header file contains common matrix and numerical operations
 * that were not provided by the dlib library and had to be implemented
 *
 *
 * Conventions:
 * Usually by 'signal' a 2-D matrix of dimensions (n, 1) i.e. a column matrix
 * is meant. 
 */


/**
 * Computes the percentile of the signal, i.e. the value of 'signal'
 * for which 'percentile' of values is less than or equal to this value
 */
double percentile (dlib::matrix<double> signal, double percentile);

/**
 * Replaces the 'to_replace' values of signal with previous values 
 * not equal to 'to_replace'
 */
void fill_with_last(dlib::matrix<double>& signal, double to_replace);

/**
 * Computes the standard deviation of the 'signal'
 * 
 * Dlib's implementation of the std computes the unbiased estimation of std for
 * the case when the mean is also estimated from the data (dividing by (n-1))
 *
 * This implementation provides the usual, 'biased' estimation (dividing by n)
 */ 
double standard_deviation(const dlib::matrix<double> &signal);

/**
 * Returns a matrix of indices of rows that don't contain any Not a Number value
 */ 
dlib::matrix<int> nonnan_rows(const dlib::matrix<double>& input);

/**
 * Computes the ratio of rows containing NaN values in the 'input' matrix.
 * Useful for debugging.
 */ 
double nan_ratio(const dlib::matrix<double>& input);

/**
 * Computes the indices of 'signal' rows with values greater than 'threshold'
 * Currently only column matrices are supported.
 */ 
dlib::matrix<int> rows_greater_than(const dlib::matrix<double> &signal, double threshold);

/**
 * Returns a matrix of indices of biggest values for each row of the matrix.
 * i.e. for an input matrix of dimension: N x M, the result matrix
 * has dimension: N x 1 and every i-th row of that matrix contains
 * the index [0,n) of the greatest value in the i-th row of the input matrix
 *
 * If you require the column version of this procedure just use 
 * dlib::trans to transpose the input matrix.
 */ 
dlib::matrix<int> argmax(const dlib::matrix<double>& input);

/**
 * Dives the 'signal' by the sum of its elements, so that the return matrix
 * sums to 1
 */ 
dlib::matrix<double> normalize(const dlib::matrix<double> &signal);

/**
 * Computes the entropy of the 'signal'. Designed to be consistent with 
 * Python's scipy.stats.entropy function
 */ 
double entropy(const dlib::matrix<double> &signal);

/**
 * Loads a matrix from the file given by filename
 */
dlib::matrix<double> load_matrix(const std::string& filename);

/**
 * Loads a matrix from input stream given by input
 */
dlib::matrix<double> load_matrix(std::istream &input);

/**
 * Converts the values from the container in the range [begin, end) to a dlib
 * matrix. Useful e.g. for converting vectors to matrices.
 */ 
template <typename I>
dlib::matrix<typename I::value_type> range_to_dlib_matrix(const I& begin, const I& end);

/**
 * Saves a matrix to a file 'filename'
 */ 
template <typename T>
void dump_matrix(const dlib::matrix<T> &data, const std::string &filename);

/**
 * Converts a vector to dlib matrix
 */ 
template <typename T>
dlib::matrix<T> vector_to_dlib_matrix(const std::vector<T> &input);

/**
 * converts a dlib matrix of type T to a std::vector<T>
 */ 
template <typename T>
std::vector<T> dlib_matrix_to_vector(const dlib::matrix<T> &input);

/**
 * Computes the logistic function of every element of the 'input' matrix
 */ 
dlib::matrix<double> logistic(const dlib::matrix<double>& input);

/**
 * Computes the softmax function for the 'input' matrix
 * useful for computing the probabilistic output of the MLP classifier
 */ 
dlib::matrix<double> softmax(const dlib::matrix<double>& input);

/**
 * Standardizes every column of matrix 'input' using the 'means' and 'stds'
 * matrices as the mean of every column and std of every column.
 */
dlib::matrix<double> standardize(const dlib::matrix<double> &input, const dlib::matrix<double> &means,
								 const dlib::matrix<double> &stds);


#endif
