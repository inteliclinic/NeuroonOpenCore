
#include <gtest/gtest.h>
#include <vector>
#include <iostream>
#include <exception>

#include <dlib/matrix.h>
#include "EegFeatures.h"
#include "Spectrogram.h"

#include "Config.h"
#include <cmath>
#include "signal_utils.h"

struct EegFeaturesTest : public ::testing::Test {
	virtual void SetUp() {
	}

	virtual void TearDown() {

	}
};

TEST_F(EegFeaturesTest, basic_sum_in_bands) {
	std::vector<double> band_borders;
	std::unique_ptr<Spectrogram> spectrogram;

	int size = 16 * 1024;
	double f = 0.125;
	int window = 512;
	int overlap = window - 64;
	dlib::matrix<double> data(size, 1);

	for (std::size_t i = 0; i != data.size(); ++i) {
		data(i,0) = sin(2 * M_PI * f * i);
	}

	band_borders = std::vector<double>({0, 0.05, 0.15, 0.2, 0.5});
	spectrogram.reset(new Spectrogram(data, 1, window, 0));

	for (int i = 0; i != band_borders.size() - 1; ++i) {
		dlib::matrix<double> band_sum = EegFeatures::sum_in_band(*spectrogram,
				band_borders[i], band_borders[i+1]);

		EXPECT_EQ(band_sum.nc(), 1);
		EXPECT_EQ(band_sum.nr(), spectrogram->size());

		const double EPS = 10e-8;
		if (f > band_borders[i] && f < band_borders[i+1]) {
			for (int i = 0; i < band_sum.nr(); ++i) {
				//std::cout << band_sum(i,0) << std::endl;
				EXPECT_TRUE(band_sum(i, 0) > EPS);
			}
		} else {
			for (int i = 0; i < band_sum.nr(); ++i) {
				//std::cout << band_sum(i,0) << std::endl;
				EXPECT_TRUE(band_sum(i, 0) < EPS);
			}
		}
	}
}

TEST_F(EegFeaturesTest, basic_rolling_mean_test) {
	const int size = 20;
	dlib::matrix<double> signal(size, 1);
	dlib::set_colm(signal, 0) =  dlib::trans(dlib::range(0, size - 1));

	const int window_size = 5;
	auto result = EegFeatures::rolling_mean(signal, window_size);
	EXPECT_EQ(result.nc(), 1);
	EXPECT_EQ(result.nr(), size);

	EXPECT_TRUE(std::isnan(result(0,0)));
	EXPECT_TRUE(std::isnan(result(1,0)));
	EXPECT_TRUE(std::isnan(result(17,0)));
	EXPECT_TRUE(std::isnan(result(18,0)));
	EXPECT_TRUE(std::isnan(result(19,0)));

	dlib::matrix<double> to_print(size, 2);
	dlib::set_colm(to_print, 0) = signal;
	dlib::set_colm(to_print, 1) = result;
	//std::cout << to_print;
}

TEST_F(EegFeaturesTest, basic_sparse_rolling_mean_test) {
	const int size = 20;
	dlib::matrix<double> signal(size, 1);
	dlib::set_colm(signal, 0) =  dlib::trans(dlib::range(0, size - 1));

	signal(5,0) = NAN;
	const int window_size = 5;
	auto result = EegFeatures::sparse_rolling_mean(signal, window_size);
	EXPECT_EQ(result.nc(), 1);
	EXPECT_EQ(result.nr(), size);

	EXPECT_TRUE(std::isnan(result(5,0)));

	EXPECT_TRUE(std::isnan(result(0,0)));
	EXPECT_TRUE(std::isnan(result(1,0)));
	EXPECT_TRUE(std::isnan(result(17,0)));
	EXPECT_TRUE(std::isnan(result(18,0)));
	EXPECT_TRUE(std::isnan(result(19,0)));


	dlib::matrix<double> to_print(size, 2);
	dlib::set_colm(to_print, 0) = signal;
	dlib::set_colm(to_print, 1) = result;
	//std::cout << to_print;
}

TEST_F(EegFeaturesTest, multiple_nans_sparse_rolling_mean_test) {
	const int size = 20;
	dlib::matrix<double> signal(size, 1);
	dlib::set_colm(signal, 0) =  dlib::trans(dlib::range(0, size - 1));

	signal(5,0) = NAN;
	signal(6,0) = NAN;
	signal(7,0) = NAN;
	signal(8,0) = NAN;
	signal(9,0) = NAN;

	const int window_size = 5;
	auto result = EegFeatures::sparse_rolling_mean(signal, window_size);
	EXPECT_EQ(result.nc(), 1);
	EXPECT_EQ(result.nr(), size);

	EXPECT_TRUE(std::isnan(result(5,0)));
	EXPECT_TRUE(std::isnan(result(6,0)));
	EXPECT_TRUE(std::isnan(result(7,0)));
	EXPECT_TRUE(std::isnan(result(8,0)));
	EXPECT_TRUE(std::isnan(result(9,0)));

	EXPECT_TRUE(std::isnan(result(0,0)));
	EXPECT_TRUE(std::isnan(result(1,0)));
	EXPECT_TRUE(std::isnan(result(17,0)));
	EXPECT_TRUE(std::isnan(result(18,0)));
	EXPECT_TRUE(std::isnan(result(19,0)));


	dlib::matrix<double> to_print(size, 2);
	dlib::set_colm(to_print, 0) = signal;
	dlib::set_colm(to_print, 1) = result;
	//std::cout << to_print;
}



TEST_F(EegFeaturesTest, basic_n_max_to_median_test) {
	dlib::matrix<double> input_data(3, 6);
	dlib::set_all_elements(input_data, 1);

	input_data(2, 0) = 2;

	auto result = EegFeatures::n_max_to_median(input_data, 3);
	//std::cout << result;


}

TEST_F(EegFeaturesTest, basic_standardize_test) {
	const int SIZE = 10;
	dlib::matrix<double> signal_to_standardize(SIZE, 1);
	for (int i = 0; i != SIZE; ++i) {
		signal_to_standardize(i, 0) = (i % 2 == 0) ? 3 : -1;
	}

	//std::cout << signal_to_standardize << std::endl;
	dlib::matrix<double> standardized = EegFeatures::standardize(signal_to_standardize);

	double st = standard_deviation(standardized);
	double m = dlib::mean(standardized);
	//std::cout << standardized << std::endl;
	EXPECT_EQ(1, st);
	EXPECT_EQ(0, m);
}

TEST_F(EegFeaturesTest, standardize_with_nans_test) {
	const int SIZE = 10;
	dlib::matrix<double> signal_to_standardize(SIZE, 1);
	for (int i = 0; i != SIZE; ++i) {
		signal_to_standardize(i, 0) = (i % 2 == 0) ? 3 : -1;
	}

	signal_to_standardize(0,0) = NAN;
	signal_to_standardize(1,0) = NAN;

	//std::cout << signal_to_standardize << std::endl;
	dlib::matrix<double> standardized = EegFeatures::standardize(signal_to_standardize);

	for (int i = 0; i != standardized.nr(); ++i) {
		bool isnan = standardized(i, 0) != standardized(i, 0);
		if (i > 1) {
			ASSERT_FALSE(isnan);
		} else {
			ASSERT_TRUE(isnan);
		}
	}
}


TEST_F(EegFeaturesTest, standardize_all_nans_test) {
	const int SIZE = 4;
	dlib::matrix<double> signal_to_standardize(SIZE, 1);
	for (int i = 0; i != SIZE; ++i) {
		signal_to_standardize(i, 0) = NAN;
	}

	//std::cout << signal_to_standardize << std::endl;
	dlib::matrix<double> standardized = EegFeatures::standardize(signal_to_standardize);

	for (int i = 0; i != standardized.nr(); ++i) {
		bool isnan = standardized(i, 0) != standardized(i, 0);
		ASSERT_TRUE(isnan);
	}
}
