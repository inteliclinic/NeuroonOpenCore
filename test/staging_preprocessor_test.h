#include "StagingPreprocessor.h"
#include <iostream>
#include <dlib/matrix.h>
#include <exception>

struct StagingPreprocessorTest: public ::testing::Test {
	virtual void SetUp() {
	}

	virtual void TearDown() {

	}
};

TEST_F(StagingPreprocessorTest, basic_transform_test) {
	int eeg_length = 2 * 1000 * 1000;
	int ir_length = eeg_length / 5;
	dlib::matrix<double> eeg_signal(eeg_length, 1);
	dlib::set_all_elements(eeg_signal, 0);

	dlib::matrix<double> ir_signal(ir_length, 1);
	dlib::set_all_elements(ir_signal, 0);

	StagingPreprocessor pre;
	dlib::matrix<double> features = pre.transform(eeg_signal, ir_signal);
	//std::cout << features << std::endl;
}



TEST_F(StagingPreprocessorTest, throws_when_not_aligned_test) {
	int eeg_length = 2 * 1000 * 1000;
	int ir_length = eeg_length / 2;
	dlib::matrix<double> eeg_signal(eeg_length, 1);
	dlib::set_all_elements(eeg_signal, 0);

	dlib::matrix<double> ir_signal(ir_length, 1);
	dlib::set_all_elements(ir_signal, 0);

	StagingPreprocessor pre;
	dlib::matrix<double> features;
	EXPECT_THROW (features = pre.transform(eeg_signal, ir_signal), std::logic_error);
	//std::cout << features << std::endl;
}
