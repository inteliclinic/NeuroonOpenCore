#include <gtest/gtest.h>

/*
 * ======= UNIT TESTS ===================
 */

#include "csvsimulator_tests.h"
#include "signal_utils_test.h"
#include "spectrogram_test.h"

#include "csvutils_tests.h"
#include "rolling_test.h"
#include "union_test.h"

#include "filter_test.h"
#include "eeg_features_test.h"
#include "staging_preprocessor_test.h"
#include "amplitude_filter_test.h"
#include "entropy_filter_test.h"
#include "multilayer_perceptron_test.h"
#include "mlp_classifier_test.h"
#include "staging_classifier_test.h"

/*
 * ====== FUNCTIONAL TESTS ===============
 */

#include "eeg_features_functional_test.h"
#include "staging_functional_test.h"

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
