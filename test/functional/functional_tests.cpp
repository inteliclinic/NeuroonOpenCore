#include <gtest/gtest.h>
#include "eeg_features_functional_test.h"
#include "staging_functional_test.h"
#include "online_staging_functional_test.h"
#include "online_staging_feature_preprocessor_test.h"
#include "online_staging_algorithm_test.h"

#include "logger.h"

int main(int argc, char **argv) {
	configure_logger();
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
