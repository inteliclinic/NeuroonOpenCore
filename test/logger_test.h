//#include <easylogging/easylogging.h>

//#define ELPP_DISABLE_LOGS
//#include "easylogging++.h"
#include <iostream>
#include <fstream>

#include "logger.h"


TEST(LoggerTest, basic_log_test) {
	LOG(INFO) << "LOGGING TEST!";
}

