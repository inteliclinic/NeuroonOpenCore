#include <dlib/matrix.h>

double percentile (dlib::matrix<double> signal, double percentile);

void fill_with_last(dlib::matrix<double>& signal, double to_replace);

double standard_deviation(const dlib::matrix<double> &signal);

dlib::matrix<int> nonnan_rows(const dlib::matrix<double>& input);

double nan_ratio(const dlib::matrix<double>& input);
