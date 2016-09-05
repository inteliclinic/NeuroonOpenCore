#ifndef SIGNAL_UTILS_H
#define SIGNAL_UTILS_H

#include <dlib/matrix.h>

double percentile (dlib::matrix<double> signal, double percentile);

void fill_with_last(dlib::matrix<double>& signal, double to_replace);

double standard_deviation(const dlib::matrix<double> &signal);

dlib::matrix<int> nonnan_rows(const dlib::matrix<double>& input);

double nan_ratio(const dlib::matrix<double>& input);

dlib::matrix<int> rows_greater_than(const dlib::matrix<double> &signal, double threshold);

dlib::matrix<int> argmax(const dlib::matrix<double>& input);

dlib::matrix<double> normalize(const dlib::matrix<double> &signal);

double entropy(const dlib::matrix<double> &signal);

dlib::matrix<double> load_matrix(const std::string& filename);

dlib::matrix<double> load_matrix(std::istream &input);

template <typename I>
dlib::matrix<typename I::value_type> range_to_dlib_matrix(const I& begin, const I& end);

template <typename T>
void dump_matrix(const dlib::matrix<T> &data, const std::string &filename);

template <typename T>
dlib::matrix<T> vector_to_dlib_matrix(const std::vector<T> &input);

template <typename T>
std::vector<T> dlib_matrix_to_vector(const dlib::matrix<T> &input);


dlib::matrix<double> logistic(const dlib::matrix<double>& input);

dlib::matrix<double> softmax(const dlib::matrix<double>& input);

dlib::matrix<double> standardize(const dlib::matrix<double> &input, const dlib::matrix<double> &means,
								 const dlib::matrix<double> &stds);


#endif
