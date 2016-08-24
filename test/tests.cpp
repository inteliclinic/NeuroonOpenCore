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
#include "expanding_mean_test.h"
#include "online_viterbi_search_test.h"
#include "easylogging_test.h"
#include "offline_staging_classifier_test.h"
/*
 * ====== FUNCTIONAL TESTS ===============
 */

#include "eeg_features_functional_test.h"
#include "staging_functional_test.h"

#include "logger.h"
ONCE_PER_APP_INITIALIZE_LOGGER

int main(int argc, char **argv) {
	configure_logger();
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
