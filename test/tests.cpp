#include <gtest/gtest.h>

#include "spectrogram_test.h"
#include "csvutils_tests.h"
#include "rolling_test.h"
#include "union_test.h"

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
