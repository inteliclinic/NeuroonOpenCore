#include "NeuroonSignalStreamApi.h"
#include <sstream>
#include "SignalSimulator.h"
#include <fstream>
#include <functional>
#include <memory>
#include <iostream>
#include <unistd.h>

/**
 * This file contains the implementation of the 'simulator' -- a program
 * for simulating the behaviour of the neuroon-alg-core library as 
 * if someone was sleeping in the mask, when in fact reading the raw
 * data from CSV files.
 *
 * This is useful for two reasons:
 *    1. It makes it possible to test the entire library from API calls to
 *       the numerical methods and machine learning concepts.
 *    2. It makes it possible to compute automatic sleep staging for
 *       signals in CSV files.
 *
 * Usage: simulator <path to directory containing RawSignal.csv and IrLedSignal.csv> [--normal] [--presentation]"
 *
 * the '--normal' option forces the time to pass at a normal rate
 *
 * the '--presentation' option starts the simulation with the online
 *     presentation mode active, so the brain_wave levels, heart rate and
 *     pulsoximetry data will be returned very frequently
 *
 * The return values of the simulation will be written to files in
 * the current working directory of the simulator process. Each time new data
 * arrives the output files are truncated and written from the beginning
 * in order to accomodate online plotting of their contents e.g. with
 * the gnuplot command.
 *
 * @author Tomasz Grel, t.grel@inteliclinic.com
 * @date October 2016
 */

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

void signal_quality_callback(SIGNAL_QUALITY sq, unsigned int size) {
  if(!size) return;
	std::stringstream ss;
  for(std::size_t i=0;i<size;i++){
    ss << sq << std::endl;
  }
	std::ofstream out("simulator_online_signal_quality.csv", std::ios_base::trunc);
	out << ss.str();
	out.flush();
	out.close();
}

void write_brain_waves(const brain_wave_levels_t* brain_waves, int size) {
	std::stringstream ss;
	for (int i = 0; i != size; ++i) {
		ss 	<< " " << brain_waves[i].delta
			<< " " << brain_waves[i].theta
			<< " " << brain_waves[i].alpha
			<< " " << brain_waves[i].beta
			<< std::endl;
	}
	std::ofstream out("brain_waves.csv", std::ios_base::trunc);
	out << ss.str();
	out.flush();
	out.close();
}

void write_pulseoximetry(const double* data, int size) {
	std::stringstream ss;
	for (int i = 0; i != size; ++i) {
		ss 	<< " " << data[i] << std::endl;
	}
	std::ofstream out("pulseoximetry.csv", std::ios_base::trunc);
	out << ss.str();
	out.flush();
	out.close();
}

void write_heart_rate(double hr) {
	std::ofstream out("hr.csv", std::ios_base::trunc);
	out << hr << std::endl;
	out.flush();
	out.close();

}

void presentation_callback(const brain_wave_levels_t* brain_waves, int bw_size, double hr, const double* pulseoximetry, int po_size) {
	write_brain_waves(brain_waves, bw_size);
	write_pulseoximetry(pulseoximetry, po_size);
	write_heart_rate(hr);
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

	NeuroonSignalProcessingState* neuroon = NULL;
	if (presentation) {
		neuroon = initialize_neuroon_alg_core(staging_callback, NULL, presentation_callback);
		start_presentation(neuroon);
	} else {
		neuroon = initialize_neuroon_alg_core(staging_callback, NULL, reinterpret_cast<presentation_callback_t>(0));
	}

	install_log_callback(neuroon, logger_callback);
	start_sleep(neuroon);

	for (int i = 0; sim.pass_time(10, speed); ++i) {

		char bytes[20];
		if (sink_sp_eeg->has_frame) {
			EegFrame f = sink_sp_eeg->take_frame();
			f.to_bytes(bytes);
			feed_data_stream0 (neuroon, bytes, 20);
		}

		if (sink_sp_ir->has_frame) {
			AccelLedsTempFrame f = sink_sp_ir->take_frame();
			f.to_bytes(bytes);
			feed_data_stream1(neuroon, bytes, 20);
		}
	}

	stop_sleep(neuroon);

	log_out->close();
}
