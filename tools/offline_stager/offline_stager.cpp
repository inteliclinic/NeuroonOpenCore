#include <iostream>
#include <dlib/matrix.h>
#include <cassert>
#include "../../src/dlib_utils.h"
#include "OfflineStagingClassifier.h"
#include "StagingPreprocessor.h"
#include "logger.h"

ONCE_PER_APP_INITIALIZE_LOGGER

int main(int argc, char** argv) {
	configure_logger();

	std::string eeg_filename(argv[1]);
	std::string ir_filename(argv[2]);
	std::string output_path(argv[3]);
	dlib::matrix<double> eeg = load_matrix(eeg_filename);
	eeg = dlib::colm(eeg, 1);

	dlib::matrix<double> ir = load_matrix(ir_filename);
	ir = dlib::colm(ir, 1);

	StagingPreprocessor pre;
	const Spectrogram eeg_spectrum = pre.get_eeg_spectrogram(eeg);
	const Spectrogram ir_spectrum = pre.get_ir_spectrogram(ir);
	dlib::matrix<double> features = pre.transform(eeg, ir);

	OfflineStagingClassifier* clf = OfflineStagingClassifier::get_instance();
	dlib::matrix<int> stages = clf->predict(features);

	dump_matrix<int>(stages, output_path + "/" + "staging.csv");
	dump_matrix<double>(eeg_spectrum.data(), output_path + "/" + "eeg.csv");
	dump_matrix<double>(ir_spectrum.data(), output_path + "/" + "ir.csv");
	dump_matrix<double>(features, output_path + "/" + "features.csv");

	return 0;
}
