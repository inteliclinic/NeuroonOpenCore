#include "fft.h"

#include <gtest/gtest.h>

#include <vector>
#include <cmath>
#include <iostream>
#include <algorithm>

struct FftTest : public ::testing::Test {

	std::vector<double> fft_result;
	double omega;
	int size;

	virtual void SetUp() {
		size = 2048;
		omega = 0.1;
		std::vector<double> data(size);
		std::vector<double> original(size);

		for (std::size_t i = 0; i != data.size(); ++i) {
			data[i] = sin(M_PI * omega * i);
			original[i] = sin(M_PI * omega * i);
		}

		fft_result = fft(data);
	}

	void TearDown() {
		;
	}
};

TEST_F(FftTest, basic_fft_test) {
	EXPECT_EQ(fft_result.size(), size);
}

TEST_F(FftTest, correct_strongest_freq) {
	auto max_it = std::max_element(fft_result.begin(), fft_result.begin() + fft_result.size() / 2);
	EXPECT_EQ(max_it - fft_result.begin(), static_cast<int> (size * omega));
}
