#include "logger.h"

ONCE_PER_APP_INITIALIZE_LOGGER

void configure_logger() {

#ifdef CAPTURE_LOGS_FROM_EASYLOGGING
	el::Helpers::installLogDispatchCallback<callback>("dummy");
#endif
	el::Loggers::addFlag(el::LoggingFlag::ColoredTerminalOutput);

	// el::Loggers::reconfigureAllLoggers(el::ConfigurationType::Format, "%datetime %level %logger %fbase:%line  %msg");
	el::Loggers::reconfigureAllLoggers(el::ConfigurationType::Format, "%datetime{%d/%M %h:%m:%s,%g} %level [%logger] %fbase:%line: %msg");
  // michal debug
	// el::Loggers::reconfigureAllLoggers(el::ConfigurationType::Format, "%datetime{%d/%M %h:%m:%s,%g} %level %logger | %func@%fbase:%line\n%msg");
	CLOG(INFO, "default") << "Logger initialized";
}
