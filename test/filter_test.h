#include <gtest/gtest.h>
#include "SpectrogramFilter.h"
#include "Spectrogram.h"

struct SpectrogramFilterTest : public ::testing::Test {

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

TEST_F(SpectrogramFilterTest, basic_filtering_test) {
	SpectrogramFilter f;
	f.filter(*spectrogram_data);

}
