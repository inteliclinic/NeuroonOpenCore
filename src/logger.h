#ifndef LOGGER_H
#define LOGGER_H


#define ELPP_DISABLE_DEBUG_LOGS

#ifdef __linux__
	#define ELPP_STACKTRACE_ON_CRASH
#endif

#define ELPP_DEFAULT_LOG_FILE "neuroon-alg-core.log"

#include "easylogging++.h"


/*
 * You can use such a structure to capture the logs
 * this may be useful e.g. when using mobile devices
 * where you can't write to a file directly using
 * the C++ standard library
 */
struct LoggingCallback : public el::LogDispatchCallback {

	std::ofstream* log_file;

	virtual ~LoggingCallback(){}


	void handle(const el::LogDispatchData* handlePtr) {
		const el::LogMessage* logMessage = handlePtr->logMessage();
		std::string message = logMessage->logger()->logBuilder()->build(logMessage, false);
		logMessage->logger()->logBuilder()->convertToColoredOutput(&message, logMessage->level());

		save_log(message);
	}

	virtual void save_log(const std::string& message) = 0;
};

#define ONCE_PER_APP_INITIALIZE_LOGGER INITIALIZE_EASYLOGGINGPP

struct DummyCallback : public LoggingCallback {
	virtual void save_log(const std::string& message) {}
};

template<typename T=DummyCallback>
void configure_logger() {
	el::Helpers::installLogDispatchCallback<T>("default");
	el::Loggers::addFlag(el::LoggingFlag::ColoredTerminalOutput);

	// el::Loggers::reconfigureAllLoggers(el::ConfigurationType::Format, "%datetime %level %logger %fbase:%line  %msg");
	el::Loggers::reconfigureAllLoggers(el::ConfigurationType::Format, "%datetime{%d/%M %h:%m:%s,%g} %level [%logger] %fbase:%line: %msg");
  // michal debug
	// el::Loggers::reconfigureAllLoggers(el::ConfigurationType::Format, "%datetime{%d/%M %h:%m:%s,%g} %level %logger | %func@%fbase:%line\n%msg");

	CLOG(INFO, "default") << "Logger initialized";
}

#endif
