#include <gtest/gtest.h>
#include <dlib/matrix.h>
#include <iostream>
#include "StagingClassifier.h"
#include "StagingPreprocessor.h"

#include "functional_tests_data.h"

TEST(StagingFunctionalTest, staging_functional_test) {
	dlib::matrix<double> features = get_python_features();
	StagingClassifier* clf = StagingClassifier::get_instance();
	dlib::matrix<int> stages = clf->predict(features);

	std::ofstream out("./functional_test_results/staging.csv");
	if(!out.good()) {
		throw std::logic_error("Could not open the file");
	}
	out << dlib::csv << stages;
}

TEST(StagingFunctionalTest, full_offline_staging_functional_test) {
	dlib::matrix<double> eeg = get_eeg_data();
	dlib::matrix<double> ir = get_ir_data();

	StagingPreprocessor pre;

	dlib::matrix<double> features = pre.transform(eeg, ir);

	StagingClassifier* clf = StagingClassifier::get_instance();
	dlib::matrix<int> stages = clf->predict(features);

	std::ofstream out("./functional_test_results/full_offline_staging.csv");
	if(!out.good()) {
		throw std::logic_error("Could not open the file");
	}
	out << dlib::csv << stages;


	std::cout << stages;
}
