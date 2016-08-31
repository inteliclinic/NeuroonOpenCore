#include "logger.h"

ONCE_PER_APP_INITIALIZE_LOGGER

FunctionPointerCallback::f_pointer FunctionPointerCallback::_callback = nullptr;

//template<typename T=FunctionPointerCallback>
void configure_logger(FunctionPointerCallback::f_pointer callback) {
	FunctionPointerCallback::_callback = callback;

	el::Helpers::installLogDispatchCallback<FunctionPointerCallback>("default");
	el::Loggers::addFlag(el::LoggingFlag::ColoredTerminalOutput);

	// el::Loggers::reconfigureAllLoggers(el::ConfigurationType::Format, "%datetime %level %logger %fbase:%line  %msg");
	el::Loggers::reconfigureAllLoggers(el::ConfigurationType::Format, "%datetime{%d/%M %h:%m:%s,%g} %level [%logger] %fbase:%line: %msg");
  // michal debug
	// el::Loggers::reconfigureAllLoggers(el::ConfigurationType::Format, "%datetime{%d/%M %h:%m:%s,%g} %level %logger | %func@%fbase:%line\n%msg");

	CLOG(INFO, "default") << "Logger initialized";
}

