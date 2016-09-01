#include "NeuroonAlgCoreApi.h"

#include "CsvSignalSimulator.h"
#include <fstream>
#include <functional>
#include <memory>

std::ofstream* staging_out;
std::ofstream* log_out;
void staging_callback(const staging_element_t* stages, int size) {

	for (int i = 0; i != size; ++i) {
		(*staging_out) << stages[i].timestamp << " " << static_cast<int>(stages[i].stage) << std::endl;
	}
}

void logger_callback(const char* message) {
	(*log_out) << message << std::endl;
}


//    std::weak_ptr<IPullBasedFrameSource<EegFrame>> weak_eeg_source(&eeg_source_sample2);
//    std::weak_ptr<IPullBasedFrameSource<AccelLedsTempFrame>> weak_ir_source(&irled_source_sample2);

//    std::weak_ptr<IDataSink<EegFrame>> dummy_eeg;
//    std::weak_ptr<IDataSink<AccelLedsTempFrame>> dummy_other;

//	auto pipe_up = std::unique_ptr<IFrameStreamPipe>(new FrameStreamPipe<EegFrame>(weak_eeg_source, dummy_eeg));
//	auto pipe2_up = std::unique_ptr<IFrameStreamPipe>(new FrameStreamPipe<AccelLedsTempFrame>(weak_ir_source, dummy_other));


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
		LOG(INFO) << "Got eeg frame: " << has_frame;
	}
};

int main() {
	std::string eeg_csv("../test/test_data/TG_190616_EEG.csv");
	std::string ir_csv("../test/test_data/TG_190616_IR.csv");

    std::shared_ptr<IPullBasedFrameSource<EegFrame>> eeg_source_sp(new CsvEegFramesSource(eeg_csv, EegFrame::Length, 1));
    std::shared_ptr<IPullBasedFrameSource<AccelLedsTempFrame>> ir_source_sp(new CsvAccelLedsTempFrameSource(ir_csv, 1));

    CsvAccelLedsTempFrameSource irled_source_sample2(ir_csv);

    CsvSignalSimulator sim;
    std::shared_ptr<EegSink> sink_sp_eeg(new EegSink());
    auto pipe_up_eeg = std::unique_ptr<IFrameStreamPipe>(new FrameStreamPipe<EegFrame>(eeg_source_sp, sink_sp_eeg));

    std::shared_ptr<IrSink> sink_sp_ir(new IrSink());
    auto pipe_up_ir = std::unique_ptr<IFrameStreamPipe>(new FrameStreamPipe<AccelLedsTempFrame>(ir_source_sp, sink_sp_ir));

	sim.add_streaming_pipe(pipe_up_eeg, EegFrame::DefaultEmissionInterval_ms);
	sim.add_streaming_pipe(pipe_up_ir, AccelLedsTempFrame::DefaultEmissionInterval_ms);

	staging_out = new std::ofstream("online_staging.csv");
	log_out = new std::ofstream("online_log.csv");

	NeuroonAlgCoreData* neuroon = initialize_neuroon_alg_core(staging_callback);
	install_log_callback(neuroon, logger_callback);

	start_sleep(neuroon);

	std::stringstream ss;
	for (int i = 0; i != 1000/*eeg_source_sp->get_frames().size()*/; ++i) {
		ss << eeg_source_sp->get_frames()[i].signal[0] << " ";
	}

	LOG(ERROR) << ss.str();

	for (int i = 0; i != 1000 * 1000; ++i) {
		sim.pass_time(10, 0);

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

	staging_out->close();
	log_out->close();
}
