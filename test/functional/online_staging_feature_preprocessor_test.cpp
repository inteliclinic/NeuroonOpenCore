#include <gtest/gtest.h>
#include <dlib/matrix.h>
#include "dlib_utils.h"
#include <fstream>
#include "logger.h"
#include "functional_tests_data.h"
#include "OnlineStagingFeaturePreprocessor.h"
#include "Spectrogram.h"
#include "Config.h"

TEST(OnlineStagingFeaturePreprocessorTest, basic_sizes_test) {
	OnlineStagingFeaturePreprocessor pre;

	dlib::matrix<double> eeg_signal = dlib::zeros_matrix<double>(2048 * 5, 1);
	dlib::matrix<double> ir_signal = dlib::zeros_matrix<double>(2048, 1);
	double seconds_since_start = 0;

	const int overlap = 0;
	const int EEG_FFT_WINDOW = 10 * 1024;
	const int IR_FFT_WINDOW = 2048;

	Spectrogram eeg_spectrogram(eeg_signal, Config::instance().neuroon_eeg_freq(), EEG_FFT_WINDOW, overlap);
	Spectrogram ir_spectrogram(ir_signal, Config::instance().neuroon_ir_freq(), IR_FFT_WINDOW, overlap);
	pre.transform(eeg_spectrogram, ir_spectrogram, seconds_since_start);
}

TEST(OnlineStagingFeaturePreprocessorTest, functional_test) {
	OnlineStagingFeaturePreprocessor pre;

	LOG(INFO) << "reading eeg signal...";
	dlib::matrix<double> eeg_signal = get_eeg_data();
	LOG(INFO) << "reading ir signal...";
	dlib::matrix<double> ir_signal = get_ir_data();

	const int EEG_WINDOW_SIZE = 1024 * 10;
	const int IR_WINDOW_SIZE = 2048;

	int eeg_index = 0;
	int ir_index = 0;
	int eeg_stride = EEG_WINDOW_SIZE / 4;
	int ir_stride = IR_WINDOW_SIZE / 4;

	std::ofstream out("./functional_test_results/online_features.csv");
	if (!out.good()) {
		throw std::logic_error("Could not open the result file");
	}

	int nrows = 1;
	while (true) {
		dlib::matrix<double> eeg_window = (dlib::rowm(eeg_signal, dlib::range(eeg_index, eeg_index + EEG_WINDOW_SIZE - 1)));
		dlib::matrix<double> ir_window = (dlib::rowm(ir_signal, dlib::range(ir_index, ir_index + IR_WINDOW_SIZE - 1)));

		LOG(DEBUG) << "eeg index: " << eeg_index << "eeg.shape = [" << eeg_window.nr() << "," << eeg_window.nc() << "]";

		double seconds_since_start = eeg_index * 0.008;

		const int overlap = 0;
		const int EEG_FFT_WINDOW = 10 * 1024;
		const int IR_FFT_WINDOW = 2048;

		Spectrogram eeg_spectrogram(eeg_window, Config::instance().neuroon_eeg_freq(), EEG_FFT_WINDOW, overlap);
		Spectrogram ir_spectrogram(ir_window, Config::instance().neuroon_ir_freq(), IR_FFT_WINDOW, overlap);

		auto preprocessed = pre.transform(eeg_spectrogram, ir_spectrogram, seconds_since_start);
		dlib::matrix<double> processed_sample = preprocessed.features;
		out << dlib::csv << processed_sample;
		LOG(DEBUG) << processed_sample;

		eeg_index += eeg_stride;
		ir_index += ir_stride;

		if (eeg_index > eeg_signal.nr() - EEG_WINDOW_SIZE) break;
		if (ir_index > ir_signal.nr() - IR_WINDOW_SIZE) break;
	}
	out.close();
}
