
#include "Spectrogram.h"

#include <gtest/gtest.h>

#include <vector>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <memory>

struct SpectrogramTest : public ::testing::Test {

	std::unique_ptr<Spectrogram> spectrogram_data;
	std::unique_ptr<Spectrogram> overlap_spectrogram;
	double omega;
	int size;
	int overlap;
	int window;

	virtual void SetUp() {
		size = 2048;
		omega = 0.1;
		window = 128;
		overlap = 64;
		dlib::matrix<double> data(size, 1);

		for (std::size_t i = 0; i != data.size(); ++i) {
			data(i,0) = sin(M_PI * omega * i);
		}

		spectrogram_data.reset(new Spectrogram(data, 1, window, 0));
		overlap_spectrogram.reset(new Spectrogram(data, 1, window, overlap));
	}

	void TearDown() {
;
	}
};

TEST_F(SpectrogramTest, correct_size) {
	EXPECT_EQ(spectrogram_data->data().nr(), size / window);
	EXPECT_EQ(spectrogram_data->data().nc(), window);
}


TEST_F(SpectrogramTest, correct_strongest_freq) {
	for (std::size_t i = 0; i != spectrogram_data->size(); ++i) {
		dlib::matrix<double> fft_result = dlib::rowm(spectrogram_data->data(), i);
		dlib::matrix<double> abs_fft = dlib::abs(fft_result);
		auto max_it = std::max_element(abs_fft.begin(), abs_fft.begin() + abs_fft.size() / 2);
		int expected_main_freq = round(omega * window / 2);

		EXPECT_EQ(expected_main_freq, max_it - abs_fft.begin());
	}
}

TEST_F(SpectrogramTest, correct_size_with_overlap) {
	EXPECT_EQ(overlap_spectrogram->data().nr(), (size - overlap) / (window - overlap));
	EXPECT_EQ(overlap_spectrogram->data().nc(), window);
}

TEST_F(SpectrogramTest, print_spectrogram_to_file) {
	std::string path("./spectrogram.csv");
	std::ofstream out(path);
	spectrogram_data->print(out);
}

//TEST_F(SpectrogramTest, big_integration_test) {
//
//}
