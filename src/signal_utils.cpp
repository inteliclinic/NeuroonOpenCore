#include "signal_utils.h"
#include <algorithm>

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

