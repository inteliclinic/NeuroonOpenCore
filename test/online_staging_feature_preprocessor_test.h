#include <gtest/gtest.h>
#include <dlib/matrix.h>
#include "dlib_utils.h"

#include "logger.h"
#include "functional_tests_data.h"
#include "OnlineStagingFeaturePreprocessor.h"

TEST(OnlineStagingFeaturePreprocessorTest, basic_sizes_test) {
	OnlineStagingFeaturePreprocessor pre;

	dlib::matrix<double> eeg_signal = dlib::zeros_matrix<double>(2048 * 5, 1);
	dlib::matrix<double> ir_signal = dlib::zeros_matrix<double>(2048, 1);
	pre.transform(eeg_signal, ir_signal);
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
	while (true) {
		dlib::matrix<double> eeg_window = (dlib::rowm(eeg_signal, dlib::range(eeg_index, eeg_index + EEG_WINDOW_SIZE - 1)));
		dlib::matrix<double> ir_window = (dlib::rowm(ir_signal, dlib::range(ir_index, ir_index + IR_WINDOW_SIZE - 1)));

		LOG(DEBUG) << "eeg index: " << eeg_index << "eeg.shape = [" << eeg_window.nr() << "," << eeg_window.nc() << "]";

		dlib::matrix<double> processed_sample = pre.transform(eeg_window, ir_window);
		LOG(INFO) << processed_sample;

		eeg_index += eeg_stride;
		ir_index += ir_stride;

		if (eeg_index > eeg_signal.nr() - EEG_WINDOW_SIZE) break;
		if (ir_index > ir_signal.nr() - IR_WINDOW_SIZE) break;
	}
}