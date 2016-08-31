#ifndef LOGGER_H
#define LOGGER_H


#define ELPP_DISABLE_DEBUG_LOGS

#ifdef __linux__
	#define ELPP_STACKTRACE_ON_CRASH
#endif

#define ELPP_DEFAULT_LOG_FILE "neuroon-alg-core.log"

#include "../3rd_party/easylogging/easylogging++.h"


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

struct FunctionPointerCallback : public LoggingCallback {

	typedef void (*f_pointer)(const char*);
	static f_pointer _callback;

	virtual void save_log(const std::string& message) {
		if (_callback != nullptr) {
			(*_callback)(message.c_str());
		}
	}
};

void configure_logger(FunctionPointerCallback::f_pointer callback = nullptr);
#endif
