#include <algorithm>
#include <cassert>
#include "dlib_utils.h"

double percentile (dlib::matrix<double> signal, double percentile) {
	std::sort(signal.begin(), signal.end());
	double result = signal(percentile * signal.nr(), 0);
	return result;
}

void fill_with_last(dlib::matrix<double>& signal, double to_replace) {
	double last_correct_value = to_replace;

	for (int i = 0; i != signal.nr(); ++i) {
		if (signal(i, 0) == to_replace || signal(i, 0) != signal(i, 0)) {
			signal(i, 0) = last_correct_value;
		} else {
			last_correct_value = signal(i, 0);
		}
	}
}

/*
 * dlib's implementation of standard deviation computes the unbiased estimator of
 * standard deviation: std(x) = 1/(n-1) * sum( (x_i - x_mean)^2 )
 *
 * here we want to have the usual estimation so that the results are consistent
 * with the onse from scikit-learn's StandardScaler, i.e.: std(x) = 1/n * sum( (x_i - x_mean)^2 )
 */
double standard_deviation(const dlib::matrix<double> &signal) {
	double mean = dlib::mean(signal);
	dlib::matrix<double> deviations =  (signal - mean);
	deviations = dlib::pointwise_multiply(deviations, deviations);

	double variance = dlib::mean(deviations);
	double stddev = sqrt(variance);
	return stddev;
}


dlib::matrix<int> nonnan_rows(const dlib::matrix<double>& input) {
	std::vector<int> v;

	for (std::size_t i = 0; i != input.nr(); ++i) {
		bool finite = dlib::is_finite(dlib::rowm(input, i));
		if (finite) {
			v.push_back(i);
		}
	}

	dlib::matrix<int> result(v.size(), 1);
	for (std::size_t i = 0; i != v.size(); ++i) {
		result(i,0) = v[i];
	}
	return result;
}


double nan_ratio(const dlib::matrix<double>& input) {
	int nans = 0;
	for (std::size_t i = 0; i != input.nr(); ++i) {
		bool finite = dlib::is_finite(dlib::rowm(input, i));
		if (!finite) {
			nans++;
		}
	}

	return double(nans) / input.nr();
}


dlib::matrix<int> rows_greater_than(const dlib::matrix<double> &signal, double threshold) {
	std::vector<int> rows;

	for (int i = 0; i != signal.nr(); ++i) {
		double value = signal(i, 0);
		if (value > threshold) {
			rows.push_back(i);
		}
	}

	dlib::matrix<int> result(rows.size(), 1);
	for (std::size_t i = 0; i != rows.size(); ++i) {
		result(i, 0) = rows[i];
	}

	return result;
}


dlib::matrix<int> argmax(const dlib::matrix<double>& input) {
	dlib::matrix<int> result(input.nr(), 1);
	for (int i = 0; i != input.nr(); ++i) {

		if (!dlib::is_finite(dlib::rowm(input, i))) {
			result(i, 0) = -1; //NaN
			continue;
		}

		int max_ind = 0;
		for (int j = 0; j != input.nc(); ++j) {
			if (input(i, j) > input(i, max_ind)) {
				max_ind = j;
			}
		}
		result(i, 0) = max_ind;
	}
	return result;
}

dlib::matrix<double> normalize(const dlib::matrix<double> &signal) {
	//TODO: make sure it works with negative values also
	double sum = dlib::sum(signal);
	return signal / sum;
}

double entropy(const dlib::matrix<double> &signal) {
	dlib::matrix<double> normalized = normalize(signal);
	dlib::matrix<double> to_sum = dlib::pointwise_multiply(normalized, dlib::log(normalized));
	double result = (-1) * dlib::sum(to_sum);
	return result;
}

std::vector<int> dlib_matrix_to_vector(const dlib::matrix<int> &input) {
	assert(input.nc() == 1);

	std::vector<int> result(input.nr());
	for (int i = 0; i != result.size(); ++i) {
		result[i] = input(i, 0);
	}
	return result;
}


dlib::matrix<double> load_matrix(const std::string& filename) {
	std::ifstream input(filename);
	if(!input.is_open()) {
		std::stringstream ss;
		ss << "Cannot open file: " << filename;
		throw std::logic_error(ss.str());
	}

	return load_matrix(input);
}

dlib::matrix<double> load_matrix(std::istream &input) {
	dlib::matrix<double> result;
	input >> result;
	return result;
}

template <typename T>
dlib::matrix<T> vector_to_dlib_matrix(const std::vector<T> &input) {
	dlib::matrix<T> result(input.size(), 1);
	for (std::size_t i = 0; i != input.size(); ++i) {
		result(i, 0) = input[i];
	}
	return result;
}
template dlib::matrix<double> vector_to_dlib_matrix<double>(const std::vector<double> &input);
template dlib::matrix<int> vector_to_dlib_matrix<int>(const std::vector<int> &input);

template <typename T>
void dump_matrix(const dlib::matrix<T> &data, const std::string &filename) {
	std::ofstream out(filename);
	if(!out.good()) {
		throw std::logic_error("Could not open the file");
	}
	out << dlib::csv << data;
	out.close();
}

template void dump_matrix<double>(const dlib::matrix<double>&, const std::string&);
template void dump_matrix<int>(const dlib::matrix<int>&, const std::string&);


dlib::matrix<double> logistic(const dlib::matrix<double>& input) {
	dlib::matrix<double> result = 1 / (1 + dlib::exp(-input));
	return result;
}

dlib::matrix<double> softmax(const dlib::matrix<double>& input) {
	dlib::matrix<double> nominator = dlib::exp(input);

	dlib::matrix<double> denominator = 1 / dlib::sum_cols(nominator);
	for (int c = 0; c != nominator.nc(); ++c) {
		dlib::set_colm(nominator, c) = dlib::pointwise_multiply(dlib::colm(nominator, c), denominator) ;
	}
	return nominator;
}

dlib::matrix<double> standardize(const dlib::matrix<double> &input, const dlib::matrix<double> &means,
								 const dlib::matrix<double> &stds) {

	//TODO: test it
	return dlib::pointwise_multiply((input - means), 1. / stds);
}
