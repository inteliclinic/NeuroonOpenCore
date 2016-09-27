#include <dlib/matrix.h>
#include "BrainWaveLevels.h"
#include <gtest/gtest.h>
#include "Spectrogram.h"

TEST(BrainWaveLevelsTest, basic_case) {
	BrainWaveLevels bw;
	dlib::matrix<double> dummy_signal = dlib::zeros_matrix<double>(256, 1);
	Spectrogram dummy_spectrogram(dummy_signal, 125, 256, 128);
	brain_wave_levels_t result = bw.predict(dummy_spectrogram).front();
	//std::cout << result.delta << "\t"
	//		  << result.theta << "\t"
	//		  << result.alpha << "\t"
	//		  << result.beta << "\n";
}

TEST(BrainWaveLevelsTest, multiple_windows) {
	BrainWaveLevels bw;
	dlib::matrix<double> dummy_signal = dlib::zeros_matrix<double>(1024, 1);
	Spectrogram dummy_spectrogram(dummy_signal, 125, 256, 128);
	std::vector<brain_wave_levels_t> result = bw.predict(dummy_spectrogram);
	ASSERT_EQ(result.size(), dummy_spectrogram.size());
}
