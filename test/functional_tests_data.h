#ifndef FUNCTIONAL_TESTS_DATA_H
#define FUNCTIONAL_TESTS_DATA_H

#include <dlib/matrix.h>

dlib::matrix<double> get_eeg_data() {
	const std::string FILENAME("../test/test_data/TG_190616_EEG.csv");
	dlib::matrix<double> data;
	std::ifstream in(FILENAME);
	if (in.good()) {
		in >> data;
		if (in.fail()) {
			throw std::logic_error("Could not read eeg data");
		}
	} else {
		std::cout << "Stream not good...\n";
		in.close();
		throw std::logic_error("Could not read eeg data");
	}

	dlib::matrix<double> signal = dlib::colm(data, 1);
	return signal;
}

#endif
