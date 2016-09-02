#include <gtest/gtest.h>
#include <dlib/matrix.h>
#include <vector>
#include <iostream>
#include "OnlineStagingAlgorithm.h"
#include "functional_tests_data.h"
#include "logger.h"
#include <fstream>
struct test_sink : public OnlineStagingAlgorithm::sink_t {
	SleepStagingResult m_last_staging;
	void consume(SleepStagingResult& res) {
		std::cout << "staging len: " << res.m_stages.size() << std::endl;
		m_last_staging = res;

		std::ofstream out("./functional_test_results/online_stages_to_plot.csv");

		for (int i = 0; i != res.m_stages.size(); ++i) {
			out << res.m_stages[i].stage << std::endl;
		}

		out << std::endl;
		out.close();
	}
};


struct mock_neuroon_signals_t : public INeuroonSignals {

	int m_step;
	std::vector<double> eeg_v;
	std::vector<double> ir_v;

	std::vector<double> m_eeg_signal;
	std::vector<double> m_ir_signal;

	mock_neuroon_signals_t() {
		m_step = 0;
		LOG(INFO) << "loading data...";
		auto eeg_signal = get_eeg_data();
		auto ir_signal = get_ir_data();

		eeg_v = dlib_matrix_to_vector<double>(eeg_signal);
		ir_v = dlib_matrix_to_vector<double>(ir_signal);
	}

	int size() {
		return eeg_v.size();
	}

	void next_step() {
		++m_step;

		int beginning_idx = std::max(0, m_step - 20000);
		m_eeg_signal.resize(m_step - beginning_idx);
		//LOG(INFO) << "samples: " << beginning_idx << ":" << m_step << std::endl;
		std::copy(eeg_v.begin() + beginning_idx, eeg_v.begin() + m_step, m_eeg_signal.begin());
		m_ir_signal.resize((m_step - beginning_idx) / 5);
		std::copy(ir_v.begin() + (beginning_idx / 5), ir_v.begin() + (m_step / 5), m_ir_signal.begin());
	}

	virtual const std::vector<double> & eeg_signal() const {
		return m_eeg_signal;
	}

	virtual const std::vector<double> & ir_led_signal() const {
		return m_ir_signal;
	}

	virtual const std::vector<double> & red_led_signal() const {
		throw std::logic_error("not implemented");
	}

	virtual const std::vector<Double3d> & accel_axes_signal() const {
		throw std::logic_error("not implemented");
	}

	virtual const std::vector<double> & temperature_signal() const {
		throw std::logic_error("not implemented");
	}

	virtual ullong last_timestamp(SignalOrigin so) const {
		return m_step * 8;
	}
	virtual std::size_t total_signal_samples(SignalOrigin ss) const {
		if (ss == EEG) {
			return m_step;
		} else {
			return m_step / 5;
		}
	}
};

TEST(OnlineStagingAlgorithmTest, basic_testcase) {
	test_sink s;

	OnlineStagingAlgorithm a({&s});

	mock_neuroon_signals_t mock_signals;
	int SAMPLES = mock_signals.size();
	for (int i = 0; i != SAMPLES; ++i) {
		mock_signals.next_step();
		a.process_input(mock_signals);
	}
	a.end_streaming(mock_signals);

	for (int i = 0; i != s.m_last_staging.m_stages.size(); ++i) {
		std::cout << s.m_last_staging.m_stages[i].stage << " ";
	}
	std::cout << std::endl;
}
