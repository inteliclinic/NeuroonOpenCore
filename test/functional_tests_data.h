#ifndef FUNCTIONAL_TESTS_DATA_H
#define FUNCTIONAL_TESTS_DATA_H

#include <dlib/matrix.h>
#include <sstream>

dlib::matrix<double> read_signal_from_file(const std::string &filename) {
	dlib::matrix<double> data;
	std::ifstream in(filename);
	if (in.good()) {
		in >> data;
		if (in.fail()) {
			std::stringstream s;
			s << "Could not read data from: " << filename;
			throw std::logic_error(s.str());
		}
	} else {
		std::cout << "Stream not good...\n";
		in.close();
		std::stringstream s;
		s << "Could not read data from: " << filename;
		throw std::logic_error(s.str());
	}

	dlib::matrix<double> signal = dlib::colm(data, 1);
	return signal;
}

dlib::matrix<double> get_eeg_data() {
	const std::string FILENAME("../test/test_data/TG_190616_EEG.csv");
	return read_signal_from_file(FILENAME);
}

dlib::matrix<double> get_ir_data() {
	const std::string FILENAME("../test/test_data/TG_190616_IR.csv");
	return read_signal_from_file(FILENAME);
}

#endif
