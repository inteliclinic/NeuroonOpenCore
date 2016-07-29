#include <gtest/gtest.h>
#include <vector>
#include <iostream>
#include <exception>

#include <dlib/matrix.h>
#include "EegFeatures.h"
#include "Spectrogram.h"

#include "Config.h"

#include "functional_tests_data.h"


std::vector<std::pair<double, double>> create_bands(const std::vector<double>& borders) {
	std::vector<std::pair<double,double>> result(borders.size() - 1);
	for (std::size_t i = 0; i != borders.size(); ++i) {
		result[i] = std::pair<double, double>(borders[i], borders[i+1]);
	}
	return result;
}

void dump_spectrogram(const Spectrogram& s) {
	std::ofstream out("./functional_test_results/spectrogram.csv");
	if(!out.good()) {
		throw std::logic_error("Could not open the file");
	}
	out << dlib::csv << s.data();
}

TEST(FunctionalTest, eeg_features_functional_test) {
	auto eeg = get_eeg_data();
	int sampling_frequency = Config::instance().neuroon_eeg_freq();
//	int window = 2048 * 5;
	int window = 2048 * 4; // was 2048*5 in python analysis, but has to be power of 2 for dlib fft
	int noverlap = window - (window / 4);

	Spectrogram spectrogram(eeg, sampling_frequency, window, noverlap);
	dump_spectrogram(spectrogram);

	//TODO: MAKE THIS CONFIGURABLE?
	std::vector<double> borders({2.5, 7.5, 10, 14, 21});
	auto bands = create_bands(borders);
	dlib::matrix<double> sums = EegFeatures::sum_in_bands(spectrogram, bands);
//	std::cout << dlib::log(sums);


}
