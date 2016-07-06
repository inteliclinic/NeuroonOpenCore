#include "fft.h"

#include <gtest/gtest.h>

#include <vector>
#include <cmath>
#include <iostream>
#include <algorithm>

struct SpectorgramTest : public ::testing::Test {

	std::vector<std::vector<double>> spectrogram_data;
	double omega;
	int size;

	virtual void SetUp() {
		size = 2048;
		omega = 0.1;
		std::vector<double> data(size);

		std::vector<std::vector<double>> res = spectrogram(data, 128);
	}

	void TearDown() {
		;
	}
};

TEST_F(SpectorgramTest, test_correct_size) {
	for (std::size_t i = 0; i != spectrogram_data.size(); ++i) {
		EXPECT_EQ(spectrogram_data[i].size(), size);
	}
}

TEST_F(SpectorgramTest, test_correct_strongest_freq) {
	for (std::size_t i = 0; i != spectrogram_data.size(); ++i) {
		auto& fft_result = spectrogram_data[i];
		auto max_it = std::max_element(fft_result.begin(), fft_result.begin() + fft_result.size() / 2);
		EXPECT_EQ(spectrogram_data[i].size(), size);
	}
}
