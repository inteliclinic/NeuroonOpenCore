#include <gtest/gtest.h>
#include <dlib/matrix.h>

#include "logger.h"
#include "OnlineStagingFeaturePreprocessor.h"

TEST(OnlineStagingFeaturePreprocessorTest, basic_sizes_test) {
	OnlineStagingFeaturePreprocessor pre;

	dlib::matrix<double> eeg_signal = dlib::zeros_matrix<double>(2048 * 5, 1);
	dlib::matrix<double> ir_signal = dlib::zeros_matrix<double>(2048, 1);
	pre.transform(eeg_signal, ir_signal);
}
