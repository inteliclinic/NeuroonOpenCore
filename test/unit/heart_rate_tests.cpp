#include "SpectrogramHeartRate.h"
#include "Spectrogram.h"

#include <dlib/matrix.h>
#include <gtest/gtest.h>
#include <vector>

TEST(SpectrogramHeartRateTest, basic_case) {

	const int N = 4;
	dlib::matrix<double> signal = dlib::zeros_matrix<double>(1024 * N, 1);
	Spectrogram test_spectrogram(signal, 25, 1024, 0);
	SpectrogramHeartRate shr;
	std::vector<double> result = shr.predict(test_spectrogram);

	ASSERT_EQ(result.size(), N);
}

