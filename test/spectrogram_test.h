
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
	double f;
	int size;
	int overlap;
	int window;

	virtual void SetUp() {
		size = 16 * 1024;
		f = 0.1;
		window = 512;
		overlap = window - 64;
		dlib::matrix<double> data(size, 1);

		for (std::size_t i = 0; i != data.size(); ++i) {
			data(i,0) = sin(2 * M_PI * f * i);
		}

		spectrogram_data.reset(new Spectrogram(data, 1, window, 0));
		overlap_spectrogram.reset(new Spectrogram(data, 1, window, overlap));
	}

	void TearDown() {
;
	}
};

TEST_F(SpectrogramTest, basic_single_fft) {
	dlib::matrix<double> fft = dlib::trans(dlib::rowm(spectrogram_data->data(), 0));
	dlib::matrix<double> frequencies = spectrogram_data->get_frequencies();

	dlib::matrix<double> full_result = dlib::matrix<double>(fft.nr(), 2);
	dlib::set_colm(full_result, 0) = frequencies;
	dlib::set_colm(full_result, 1) = fft;

	int frequency_closest_to_f = dlib::index_of_min(dlib::abs(frequencies - f));
	int largest_fft_val = dlib::index_of_max(fft);
	EXPECT_EQ(frequency_closest_to_f, largest_fft_val);
}

TEST_F(SpectrogramTest, correct_size_with_overlap) {
	EXPECT_EQ(overlap_spectrogram->data().nr(), (size - overlap) / (window - overlap));
	EXPECT_EQ(overlap_spectrogram->data().nc(), window / 2);
}

TEST_F(SpectrogramTest, basic_get_band) {
	dlib::matrix<double> small_band = spectrogram_data->get_band(0.0, 1.0);
	EXPECT_EQ(small_band.nc(), spectrogram_data->data().nc());
	EXPECT_EQ(small_band.nr(), spectrogram_data->data().nr());
}

TEST_F(SpectrogramTest, small_band) {
	dlib::matrix<double> small_band = spectrogram_data->get_band(0.124999, 0.125001);
	EXPECT_EQ(small_band.nc(), 1);
	EXPECT_EQ(small_band.nr(), spectrogram_data->data().nr());
}

TEST_F(SpectrogramTest, basic_create_from_band) {
	Spectrogram band = spectrogram_data->create_from_band(0.124999, 0.125001);

	EXPECT_EQ(band.data().nc(), 1);
	EXPECT_EQ(band.data().nr(), spectrogram_data->data().nr());
	EXPECT_EQ(band.get_frequencies()(0,0), 0.125);
}

TEST_F(SpectrogramTest, basic_moment1) {
	dlib::matrix<double> moment1 = spectrogram_data->compute_moment(1);
	EXPECT_EQ(moment1.size(), spectrogram_data->data().nr());
	for (int i = 0; i != moment1.size(); ++i) {
		EXPECT_TRUE(moment1(i, 0) != 0);
	}
}

TEST_F(SpectrogramTest, overlap_moment) {
	dlib::matrix<double> moment1 = overlap_spectrogram->compute_moment(1);
	EXPECT_EQ(moment1.size(), overlap_spectrogram->data().nr());
	for (int i = 0; i != moment1.size(); ++i) {
		EXPECT_TRUE(moment1(i, 0) != 0);
	}
}


TEST_F(SpectrogramTest, moment10) {
	dlib::matrix<double> moment10 = spectrogram_data->compute_moment(10);
	EXPECT_EQ(moment10.size(), spectrogram_data->data().nr());
	for (int i = 0; i != moment10.size(); ++i) {
		const double TOL = 0.001;
		double diff = std::abs(moment10(i, 0) - f);
		EXPECT_TRUE(diff < TOL);
	}
}

TEST_F(SpectrogramTest, print_spectrogram_to_file) {
	std::string path("./spectrogram.csv");
	std::ofstream out(path);
	spectrogram_data->print(out);
}



//TEST_F(SpectrogramTest, big_integration_test) {
//
//}
