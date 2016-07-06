#include "fft.h"
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
		std::vector<double> data(size);

		for (std::size_t i = 0; i != data.size(); ++i) {
			data[i] = sin(M_PI * omega * i);
		}

		spectrogram_data.reset(new Spectrogram(data, 1, window, 0));
		overlap_spectrogram.reset(new Spectrogram(data, 1, window, overlap));
	}

	void TearDown() {
;
	}
};

TEST_F(SpectrogramTest, correct_size) {
	for (std::size_t i = 0; i != spectrogram_data->size(); ++i) {
		EXPECT_EQ(spectrogram_data->fft_at(i).size(), window);
	}
}

std::vector<double> to_absolute_val(const std::vector<double>& data){
	std::vector<double> result(data.size());
	for (std::size_t i = 0; i != data.size(); ++i) {
		result[i] = fabs(data[i]);
	}
	return result;
}

TEST_F(SpectrogramTest, correct_strongest_freq) {
	for (std::size_t i = 0; i != spectrogram_data->size(); ++i) {
		const auto& fft_result = spectrogram_data->fft_at(i);
		auto abs_fft = to_absolute_val(fft_result);
		auto max_it = std::max_element(abs_fft.begin(), abs_fft.begin() + abs_fft.size() / 2);
		int expected_main_freq = round(omega * window);

		bool success = expected_main_freq == max_it - abs_fft.begin() ||
				expected_main_freq == 1 + max_it - abs_fft.begin();

		EXPECT_TRUE(success);
	}
}

TEST_F(SpectrogramTest, correct_size_with_overlap) {
	for (std::size_t i = 0; i != overlap_spectrogram->size(); ++i) {
		EXPECT_EQ(overlap_spectrogram->fft_at(i).size(), window);
	}
}

TEST_F(SpectrogramTest, number_of_windows_overlap) {
	EXPECT_EQ(overlap_spectrogram->size(), size / (window - overlap));
}

TEST_F(SpectrogramTest, print_spectrogram_to_file) {
	std::string path("./spectrogram.csv");
	std::ofstream out(path);
	spectrogram_data->print(out);
}
