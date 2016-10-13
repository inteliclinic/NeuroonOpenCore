#include "NeuroonAlgCoreApi.h"

#include "NeuroonSignalFrames.h"
#include <fstream>
#include <functional>
#include <memory>
#include <iostream>
#include <unistd.h>

/**
 * This file contains the implementation of the 'parser' program for parsing
 * raw neuroon continous transmission data into CSV files.
 * 
 * Usage: parser <binary eeg frames file> <binary acc,temp,ir frames file>
 * 
 * Output:
 *    -- RawSignal.csv -- EEG signal frames
 *    -- IrLedSignal.csv -- Infrared LED signal
 *    -- RedLedSignal.csv -- Red LED signal
 *    -- AccelAxes.csv -- Accelerometer data
 *    -- Termometer.csv -- Termometer data
 */ 

void parse_eeg(const std::string& filename) {
	std::ifstream eeg_in(filename);

	if (!eeg_in.is_open()) {
		std::cout << "The file: " << filename << " won\'t be parsed -- could not open" << std::endl;
		return;
	}

	std::ofstream eeg_parsed("./RawSignal.csv", std::ios_base::trunc);
	const int SIZE = 20;
	char buffer[20];
	eeg_parsed << "timestamp,signal" << std::endl;
	while (eeg_in.good()) {
		eeg_in.read(buffer, SIZE);
		auto frame = EegFrame::from_bytes_array(buffer, SIZE);
		for (int i = 0; i != frame.Length; ++i) {
			eeg_parsed << frame.timestamp + (8 * i) << "," << frame.signal[i] << std::endl;
		}
	}
	eeg_in.close();
	eeg_parsed.close();
}


void parse_other(const std::string& filename) {
	std::ifstream other_in(filename);

	if (!other_in.is_open()) {
		std::cout << "The file: " << filename << " won\'t be parsed -- could not open" << std::endl;
		return;
	}

	std::ofstream ir_parsed("./IrLedSignal.csv");
	std::ofstream red_parsed("./RedLedSignal.csv");
	std::ofstream acc_parsed("./AccelAxes.csv");
	std::ofstream termo_parsed("./Termometer.csv");
	const int SIZE = 20;
	char buffer[20];
	ir_parsed << "timestamp,signal" << std::endl;
	red_parsed << "timestamp,signal" << std::endl;
	acc_parsed << "timestamp,x,y,z" << std::endl;
	termo_parsed << "timestamp, temp_0, temp_1" << std::endl;
	while (other_in.good()) {
		other_in.read(buffer, SIZE);
		AccelLedsTempFrame frame = AccelLedsTempFrame::from_bytes_array(buffer, SIZE);
		ir_parsed << frame.timestamp << "," << frame.ir_led << std::endl;
		red_parsed << frame.timestamp << "," << frame.red_led << std::endl;
		acc_parsed << frame.timestamp << "," << frame.accel_axes.x << ","  << frame.accel_axes.y << "," << frame.accel_axes.z << std::endl;
		termo_parsed << frame.timestamp << "," << frame.temperature[0] << "," << frame.temperature[1] << std::endl;
	}

	other_in.close();
	ir_parsed.close();
	red_parsed.close();
	acc_parsed.close();
	termo_parsed.close();
}

int main(int argc, char** argv) {
	if (argc != 3) {
		std::cout << "Usage: parser <binary eeg frames file> <binary acc,temp,ir frames file>" << std::endl;
		return -1;
	}

	parse_eeg(argv[1]);
	parse_other(argv[2]);
}
