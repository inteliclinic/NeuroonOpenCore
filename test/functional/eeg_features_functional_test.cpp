#include <gtest/gtest.h>
#include <vector>
#include <iostream>
#include <exception>
#include <chrono>

#include <dlib/matrix.h>
#include "EegFeatures.h"
#include "Spectrogram.h"
#include "StagingPreprocessor.h"

#include "Config.h"
#include "ExecutionTime.h"

#include "functional_tests_data.h"


void dump_spectrogram(const Spectrogram& s) {
	std::ofstream out("./functional_test_results/spectrogram.csv");
	if(!out.good()) {
		throw std::logic_error("Could not open the file");
	}
	out << dlib::csv << s.data();
}

TEST(FunctionalTest, spectrogram_computation_functional_test) {
	dlib::matrix<double>* eeg = nullptr;
	//dlib::matrix<double>* ir = nullptr;
	{
		ExecutionTime t("Reading EEG data");
		eeg = new dlib::matrix<double> (get_eeg_data());
		//ir = new dlib::matrix<double> (get_ir_data());
	}
	int sampling_frequency = Config::instance().neuroon_eeg_freq();
//	int window = 2048 * 5;
	int window = 2048 * 4; // was 2048*5 in python analysis, but has to be power of 2 for dlib fft
	int noverlap = window - (window / 4);

	Spectrogram* spectrogram = nullptr;
	{
		ExecutionTime t("Computing the spectrogram");
		spectrogram = new Spectrogram(*eeg, sampling_frequency, window, noverlap);
	}

	{
		ExecutionTime t("Saving the results");
		dump_spectrogram(*spectrogram);
	}
	delete spectrogram;
	delete eeg;

//	//TODO: MAKE THIS CONFIGURABLE?
//	std::vector<double> borders({2.5, 7.5, 10, 14, 21});
//	auto bands = create_bands(borders);
//	dlib::matrix<double> sums = EegFeatures::sum_in_bands(spectrogram, bands);
//	std::cout << dlib::log(sums);


}


TEST(FunctionalTest, full_preprocessing_functional_test) {
	dlib::matrix<double> eeg = get_eeg_data();
	dlib::matrix<double> ir = get_ir_data();

	StagingPreprocessor pre;
	dlib::matrix<double> features = pre.transform(eeg, ir);
	std::ofstream out("./functional_test_results/features.csv");
	if(!out.good()) {
		throw std::logic_error("Could not open the file");
	}
	std::cout << "Before dump: " << nan_ratio(features) << std::endl;
	out << dlib::csv << features;

}
