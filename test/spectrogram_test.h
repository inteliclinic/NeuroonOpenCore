#include "fft.h"

#include <gtest/gtest.h>

#include <vector>
#include <cmath>
#include <iostream>
#include <algorithm>

struct SpectrogramTest : public ::testing::Test {

	std::vector<std::vector<double>> spectrogram_data;
	std::vector<std::vector<double>> overlap_spectrogram_data;
	double omega;
	int size;
	int overlap;
	int window;

	virtual void SetUp() {
		size = 2048;
		omega = 0.1;
		window = 128;
		overlap = 64;
		std::vector<double> data(size);

		spectrogram_data = spectrogram(data, window, 0);
		overlap_spectrogram_data = spectrogram(data, window, overlap);
	}

	void TearDown() {
		;
	}
};

TEST_F(SpectrogramTest, correct_size) {
	for (std::size_t i = 0; i != spectrogram_data.size(); ++i) {
		EXPECT_EQ(spectrogram_data[i].size(), window);
	}
}


TEST_F(SpectrogramTest, correct_strongest_freq) {
	for (std::size_t i = 0; i != spectrogram_data.size(); ++i) {
		auto& fft_result = spectrogram_data[i];
		auto max_it = std::max_element(fft_result.begin(), fft_result.begin() + fft_result.size() / 2);
		EXPECT_EQ(spectrogram_data[i].size(), window);
	}
}


TEST_F(SpectrogramTest, correct_size_with_overlap) {
	for (std::size_t i = 0; i != overlap_spectrogram_data.size(); ++i) {
		EXPECT_EQ(overlap_spectrogram_data[i].size(), window);
	}
}

TEST_F(SpectrogramTest, number_of_windows_overlap) {
	EXPECT_EQ(overlap_spectrogram_data.size(), size / (window - overlap));
}
