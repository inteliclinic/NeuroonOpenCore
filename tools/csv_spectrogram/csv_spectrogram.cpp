/*
 * csv_spectrogram.cpp
 *
 *  Created on: Jul 7, 2016
 *      Author: tomek
 */
#include <iostream>
#include <dlib/matrix.h>
#include <complex>
#include "../../src/SimpleSpectrogramFilter.h"

#include "Spectrogram.h"
#include "signal_utils.h"
#include "logger.h"

ONCE_PER_APP_INITIALIZE_LOGGER

int main(int argc, char* argv[]) {
	configure_logger();

	if (argc < 3) {
		std::cout << "Filename argument is required\n";
		return -1;
	}

	std::string filename = argv[1];

	std::string command = argv[2];

	dlib::matrix<double> data(10, 1);
	std::ifstream in(filename);
	if (in.good()) {
		in >> data;
		if (in.fail()) {
			std::cout << "FAIL!!!\n";
		}
	} else {
		std::cout << "Stream not good...\n";
		in.close();
		return -1;
	}

	dlib::matrix<double> signal = dlib::colm(data, 1);

	int window = 1024 * 2;
	const int overlap = window - 256;
	const int sampling_f = 25;

	double low = 0.6;
	double high = 1.4;

	Spectrogram s(signal, sampling_f, window, overlap);
	s = s.create_from_band(low, high);
	SimpleSpectrogramFilter f;
	f.filter(s);

	if (command == "spectrogram") {
		s.print(std::cout);
	} else if (command == "moment"){
		std::string arg_moment = argv[3];
		int degree = std::stoi(arg_moment);
		dlib::matrix<double> moment = s.compute_moment(degree);
		fill_with_last(moment, f.INCORRECT);
		std::cout << dlib::csv << moment << std::endl;
	}
}
