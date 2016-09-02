#include <gtest/gtest.h>
#include "logger.h"

int main(int argc, char **argv) {
	configure_logger();
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
