#include <gtest/gtest.h>
#include "StagingClassifier.h"

TEST(StagingClassifierTest, create_the_classifier_test) {
	StagingClassifier* clf = StagingClassifier::get_instance();

	const int NUMBER_OF_FEATURES = 7;
	const int ROWS = 100;
	dlib::matrix<double> dummy_features(ROWS, NUMBER_OF_FEATURES);

	dlib::matrix<int> stages = clf->predict(dummy_features);
	EXPECT_EQ(stages.nr(), ROWS);
	EXPECT_EQ(stages.nc(), 1);
	//std::cout << stages;
}

