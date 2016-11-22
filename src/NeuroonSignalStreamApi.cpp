/**
 * This file contains the implementation of the public API for the
 * signal streamining part of the library. The implementation uses C++ constructions
 * please make sure this constructions do not 'leak' to the header
 * file containing the API, as the API is supposed to be pure C.
 *
 * @author Tomasz Grel, t.grel@inteliclinic.com
 * @date October 2016
 */

#include <memory>
#include <sstream>

#include "NeuroonSignalStreamApi.h"
#include "AlgCoreDaemon.h"
#include "OnlineStagingAlgorithm.h"
#include "OnlinePresentationAlgorithm.h"
#include "logger.h"

struct NeuroonSignalProcessingState {
	AlgCoreDaemon _daemon;
	OnlinePresentationAlgorithm* _online_presentation;
};

struct LoggingSink : public OnlineStagingAlgorithm::sink_t {
	SleepStagingResult m_last_staging;

	virtual ~LoggingSink() {}

	void consume(SleepStagingResult& res) {
		m_last_staging = res;

		std::stringstream ss;
		for (int i = 0; i != res.m_stages.size(); ++i) {
			ss << res.m_stages[i].stage << " ";
		}
		LOG(INFO) << "online staging: " << ss.str();
	}
};

struct CallbackStagingSink : public OnlineStagingAlgorithm::sink_t {
	virtual ~CallbackStagingSink() {}

	ncStagingCallback _callback;
	CallbackStagingSink(ncStagingCallback callback) {
		_callback = callback;
	}

	void consume(SleepStagingResult& res) {
		(*_callback)(res.m_stages.data(), res.m_stages.size());
	}
};

struct CallbackPresentationSink : public OnlinePresentationAlgorithm::sink_t {
	virtual ~CallbackPresentationSink() {}

	ncPresentationCallback _callback;
	CallbackPresentationSink(ncPresentationCallback callback) {
		_callback = callback;
	}

	void consume(OnlinePresentationResult& res) {
		(*_callback)(res.brain_waves, res.bw_size,
				     res.heart_rate, res.pulse_data, res.pd_size);
	}
};

NeuroonSignalProcessingState *ncInitializeNeuroonAlgCore(ncStagingCallback staging_callback,
                                                          ncSignalQualityCallback sq_callback,
                                                          ncPresentationCallback presentation_callback) {
	LOG(INFO) << "API CALL";
	NeuroonSignalProcessingState* data = new NeuroonSignalProcessingState();
	data->_online_presentation = nullptr;

	LoggingSink* ls = new LoggingSink();
	CallbackStagingSink* css = new CallbackStagingSink(staging_callback);
	auto online_alg = std::unique_ptr<IStreamingAlgorithm>(new OnlineStagingAlgorithm({ls, css}));
	data->_daemon.add_streaming_algorithms(online_alg);

	if (reinterpret_cast<long> (presentation_callback) != 0) {
		CallbackPresentationSink* ps = new CallbackPresentationSink(presentation_callback);
		auto presentation_alg = new OnlinePresentationAlgorithm({ps});
		auto alg_ptr = std::unique_ptr<IStreamingAlgorithm>(presentation_alg);
		data->_daemon.add_streaming_algorithms(alg_ptr);
		data->_online_presentation = presentation_alg;
	}

	LOG(INFO) << "API CALL END";
	return data;
}

bool ncDestroyNeuroonAlgCore(NeuroonSignalProcessingState* data) {
	LOG(INFO) << "API CALL";
	delete data;
	LOG(INFO) << "API CALL END";
    return true;
}

bool ncStartSleep(NeuroonSignalProcessingState* data) {
	LOG(INFO) << "API CALL";
	data->_daemon.start_processing();
	LOG(INFO) << "API CALL END";
    return true;
}

bool ncStopSleep(NeuroonSignalProcessingState* data) {
	LOG(INFO) << "API CALL";
	data->_daemon.end_processing();
	LOG(INFO) << "API CALL END";
    return true;
}

bool ncFeedDataStream0(NeuroonSignalProcessingState* data, char* bytes, int size) {
	LOG(DEBUG) << "API CALL";

	NeuroonFrameBytes frame;
	frame.bytes = bytes;
	frame.size = size;
	frame.source_stream = NeuroonFrameBytes::SourceStream::EEG;
	data->_daemon.consume(frame);

	LOG(DEBUG) << "API CALL END";
    return true;
}

bool ncFeedDataStream1(NeuroonSignalProcessingState* data, char* bytes, int size) {
	LOG(DEBUG) << "API CALL";

	NeuroonFrameBytes frame;
	frame.bytes = bytes;
	frame.size = size;
	frame.source_stream = NeuroonFrameBytes::SourceStream::ALT;
	data->_daemon.consume(frame);

	LOG(DEBUG) << "API CALL END";
    return true;
}

bool ncFeedDataStream2(NeuroonSignalProcessingState* data, char* bytes, int size) {
	LOG(DEBUG) << "API CALL -- NOT USED CURRENTLY";
    return true;
}

bool ncInstallLogCallback(NeuroonSignalProcessingState* data, ncLoggerCallback callback) {
	LOG(INFO) << "API CALL";
	configure_logger(callback);
	LOG(INFO) << "API CALL END";
    return true;
}

bool ncStartPresentation(NeuroonSignalProcessingState* data) {
	LOG(INFO) << "API CALL";

	if (!data->_online_presentation) {
		return false;
	}

	data->_online_presentation->activate();
	LOG(INFO) << "API CALL END";
    return true;
}

bool ncStopPresentation(NeuroonSignalProcessingState* data) {
	LOG(INFO) << "API CALL";

	if (!data->_online_presentation) {
		return false;
	}

	data->_online_presentation->deactivate();
	LOG(INFO) << "API CALL END";
    return true;
}
