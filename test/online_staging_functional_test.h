#include <gtest/gtest.h>
#include <dlib/matrix.h>
#include <iostream>
#include "OnlineStagingClassifier.h"
#include "StagingPreprocessor.h"
#include <vector>
#include "functional_tests_data.h"

void replace_to_nan(double to_replace, dlib::matrix<double> &m) {
	for (int r = 0; r != m.nr(); ++r) {
		for (int c = 0; c != m.nc(); ++c) {
			if (m(r,c) != to_replace) {
				continue;
			}
			m(r,c) = NAN;
		}
	}
}

TEST(OnlineStagingFunctionalTest, online_staging_functional_test) {
	dlib::matrix<double> features = get_online_python_features();
	replace_to_nan(4096, features);
	EXPECT_TRUE(features.nr() > 100);

	OnlineStagingClassifier clf;
	std::cout << features.nr() << "x"<< features.nc() << std::endl;
	std::vector<int> stages;
	for (int r = 0; r != features.nr(); ++r) {
		dlib::matrix<double> sample = dlib::rowm(features, r);
		stages = clf.predict(sample);
	}
	stages = clf.stop();


	std::ofstream out("./functional_test_results/online_staging.csv");
	if(!out.good()) {
		throw std::logic_error("Could not open the file");
	}
	dlib::matrix<int> matrix_stages = vector_to_dlib_matrix<int>(stages);
	out << dlib::csv << matrix_stages;
}

TEST(OnlineStagingFunctionalTest, full_online_staging_functional_test) {
	dlib::matrix<double> eeg_signal = get_eeg_data();
	dlib::matrix<double> ir_signal = get_ir_data();

	OnlineStagingClassifier clf;

	const int EEG_WINDOW_SIZE = 1024 * 10;
	const int IR_WINDOW_SIZE = 2048;

	int eeg_index = 0;
	int ir_index = 0;
	int eeg_stride = EEG_WINDOW_SIZE / 4;
	int ir_stride = IR_WINDOW_SIZE / 4;

	int nrows = 1;
	std::vector<int> stages;
	while (true) {
		dlib::matrix<double> eeg_window = (dlib::rowm(eeg_signal, dlib::range(eeg_index, eeg_index + EEG_WINDOW_SIZE - 1)));
		dlib::matrix<double> ir_window = (dlib::rowm(ir_signal, dlib::range(ir_index, ir_index + IR_WINDOW_SIZE - 1)));

		LOG(DEBUG) << "eeg index: " << eeg_index << "eeg.shape = [" << eeg_window.nr() << "," << eeg_window.nc() << "]";

		double seconds_since_start = eeg_index * 0.008;
		stages = clf.step(eeg_window, ir_window, seconds_since_start);

		LOG(DEBUG) << "classified: "<< stages.size();
		eeg_index += eeg_stride;
		ir_index += ir_stride;

		if (eeg_index > eeg_signal.nr() - EEG_WINDOW_SIZE) break;
		if (ir_index > ir_signal.nr() - IR_WINDOW_SIZE) break;
	}
	stages = clf.stop();
	dlib::matrix<int> matrix_stages = vector_to_dlib_matrix<int>(stages);
	std::ofstream out("./functional_test_results/full_online_staging.csv");
	if(!out.good()) {
		throw std::logic_error("Could not open the file");
	}
	out << matrix_stages;
}
