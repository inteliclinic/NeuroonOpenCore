#include <memory>

#include "NeuroonAlgCoreApi.h"

#include "AlgCoreDaemon.h"
#include "OnlineStagingAlgorithm.h"
#include "logger.h"
#include <sstream>
struct NeuroonAlgCoreData {
	AlgCoreDaemon _daemon;
};

struct logging_sink : public OnlineStagingAlgorithm::sink_t {
	SleepStagingResult m_last_staging;

	virtual ~logging_sink() {}

	void consume(SleepStagingResult& res) {
		m_last_staging = res;

		std::stringstream ss;
		for (int i = 0; i != res.m_stages.size(); ++i) {
			ss << res.m_stages[i].stage << " ";
		}
		LOG(INFO) << "online staging: " << ss.str();
	}
};

struct callback_staging_sink : public OnlineStagingAlgorithm::sink_t {
	virtual ~callback_staging_sink() {}

	staging_callback_t _callback;
	callback_staging_sink(staging_callback_t callback) {
		_callback = callback;
	}

	void consume(SleepStagingResult& res) {
		(*_callback)(res.m_stages.data(), res.m_stages.size());
	}
};

NeuroonAlgCoreData* initialize_neuroon_alg_core(staging_callback_t staging_callback) {
	LOG(INFO) << "API CALL";
	NeuroonAlgCoreData* data = new NeuroonAlgCoreData();

	logging_sink* ls = new logging_sink();
	callback_staging_sink* css = new callback_staging_sink(staging_callback);
	auto online_alg = std::unique_ptr<IStreamingAlgorithm>(new OnlineStagingAlgorithm({ls, css}));
	data->_daemon.add_streaming_algorithms(online_alg);

	LOG(INFO) << "API CALL END";
	return data;
}

void destroy_neuroon_alg_core(NeuroonAlgCoreData* data) {
	LOG(INFO) << "API CALL";
	delete data;
	LOG(INFO) << "API CALL END";
}

void start_sleep(NeuroonAlgCoreData* data) {
	LOG(INFO) << "API CALL";
	data->_daemon.start_processing();
	LOG(INFO) << "API CALL END";
}


void feed_eeg_data(NeuroonAlgCoreData* data, char* bytes, int size) {
	LOG(INFO) << "API CALL";

	NeuroonFrameBytes frame;
	frame.bytes = bytes;
	frame.size = size;
	frame.source_stream = NeuroonFrameBytes::SourceStream::EEG;
	data->_daemon.consume(frame);

	LOG(INFO) << "API CALL END";
}

void feed_ir_led_data(NeuroonAlgCoreData* data, char* bytes, int size) {
	LOG(INFO) << "API CALL";

	NeuroonFrameBytes frame;
	frame.bytes = bytes;
	frame.size = size;
	frame.source_stream = NeuroonFrameBytes::SourceStream::ALT;
	data->_daemon.consume(frame);

	LOG(INFO) << "API CALL END";
}

void install_log_callback(NeuroonAlgCoreData* data, logger_callback_t callback) {
	LOG(INFO) << "API CALL";
	configure_logger(callback);
	LOG(INFO) << "API CALL END";
}

