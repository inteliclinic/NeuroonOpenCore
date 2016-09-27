#include "NeuroonAlgCoreApi.h"
#include <sstream>
#include "SignalSimulator.h"
#include <fstream>
#include <functional>
#include <memory>
#include <iostream>
#include <unistd.h>

std::ofstream* log_out;

void staging_callback(const staging_element_t* stages, int size) {
	std::stringstream ss;
	for (int i = 0; i != size; ++i) {
		ss << stages[i].timestamp << " " << static_cast<int>(stages[i].stage)
			<< " " << static_cast<int>(stages[i].signal_quality)
			<< " " << stages[i].brain_waves.delta
			<< " " << stages[i].brain_waves.theta
			<< " " << stages[i].brain_waves.alpha
			<< " " << stages[i].brain_waves.beta
			<< std::endl;
	}
	std::ofstream out("simulator_online_staging.csv", std::ios_base::trunc);
	out << ss.str();
	out.flush();
	out.close();
}

void presentation_callback(const presentation_element_t* data, int size) {
	std::stringstream ss;
	for (int i = 0; i != size; ++i) {
		ss << data->heart_rate
			<< " " << data[i].brain_waves.delta
			<< " " << data[i].brain_waves.theta
			<< " " << data[i].brain_waves.alpha
			<< " " << data[i].brain_waves.beta
			<< std::endl;
	}
	std::ofstream out("presentation.csv", std::ios_base::trunc);
	out << ss.str();
	out.flush();
	out.close();
}

void logger_callback(const char* message) {
	(*log_out) << message << std::endl;
}

template <typename T>
class DummyDataSink : public IDataSink<T> {
	virtual void consume(T&) {}
};

struct IrSink : public IDataSink<AccelLedsTempFrame> {
	AccelLedsTempFrame m_frame;
	bool has_frame = false;

	AccelLedsTempFrame take_frame() {
		has_frame = false;
		return m_frame;
	}

	virtual void consume(AccelLedsTempFrame &frame) {
		m_frame = frame;
		has_frame = true;
	}

};


struct EegSink : public IDataSink<EegFrame> {

	EegFrame m_frame;
	bool has_frame = false;

	EegFrame take_frame() {
		has_frame = false;
		return m_frame;
	}

	virtual void consume(EegFrame &frame) {
		has_frame = true;
		m_frame = frame;
	}
};

int main(int argc, char** argv) {

	if (argc < 2) {
		std::cout << "Usage: simulator <path to directory containing RawSignal.csv and IrLedSignal.csv> [--normal] [--presentation]" << std::endl;
		return -1;
	}

	bool presentation = false;
	double speed = 0;
	std::string directory(argv[1]);
	for (int i = 0; i != argc; ++i) {
		std::string arg(argv[i]);
		if (arg == "--normal") {
			speed = 1;
		} else if (arg == std::string("--presentation")) {
			presentation = true;
		}
	}

	std::string eeg_csv(directory + "/RawSignal.csv");
	std::string ir_csv(directory + "IrLedSignal.csv");

	auto eeg_source = new EegFramesSource(eeg_csv, "signal");
    std::shared_ptr<IPullBasedOfflineSource<EegFrame>> eeg_source_sp(eeg_source);
    std::shared_ptr<IPullBasedOfflineSource<AccelLedsTempFrame>> ir_source_sp(new AccelLedsTempFrameSource(SignalSource<std::int32_t>::csv_column(ir_csv, "signal")));

    AccelLedsTempFrameSource irled_source_sample2(SignalSource<std::int32_t>::csv_column(ir_csv, "signal"));

    SignalSimulator sim;
    std::shared_ptr<EegSink> sink_sp_eeg(new EegSink());
    auto pipe_up_eeg = std::unique_ptr<IFrameStreamPipe>(new FrameStreamPipe<EegFrame>(eeg_source_sp, sink_sp_eeg));

    std::shared_ptr<IrSink> sink_sp_ir(new IrSink());
    auto pipe_up_ir = std::unique_ptr<IFrameStreamPipe>(new FrameStreamPipe<AccelLedsTempFrame>(ir_source_sp, sink_sp_ir));

    sim.add_streaming_pipe(std::move(pipe_up_eeg), EegFrame::DefaultEmissionInterval_ms);
    sim.add_streaming_pipe(std::move(pipe_up_ir), AccelLedsTempFrame::DefaultEmissionInterval_ms);

	log_out = new std::ofstream("simulator_log.csv", std::ios_base::trunc);

	NeuroonAlgCoreData* neuroon = NULL;
	if (presentation) {
		neuroon = initialize_neuroon_alg_core(staging_callback, presentation_callback);
		start_presentation(neuroon);
	} else {
		neuroon = initialize_neuroon_alg_core(staging_callback, reinterpret_cast<presentation_callback_t>(0));
	}

	install_log_callback(neuroon, logger_callback);
	start_sleep(neuroon);

	for (int i = 0; sim.pass_time(10, speed); ++i) {

		char bytes[20];
		if (sink_sp_eeg->has_frame) {
			EegFrame f = sink_sp_eeg->take_frame();
			f.to_bytes(bytes);
			feed_eeg_data (neuroon, bytes, 20);
		}

		if (sink_sp_ir->has_frame) {
			AccelLedsTempFrame f = sink_sp_ir->take_frame();
			f.to_bytes(bytes);
			feed_ir_led_data(neuroon, bytes, 20);
		}
	}

	stop_sleep(neuroon);

	log_out->close();
}
