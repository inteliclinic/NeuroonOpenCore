#ifndef FUNCTIONAL_TESTS_DATA_H
#define FUNCTIONAL_TESTS_DATA_H

#include "dlib_utils.h"
#include <dlib/matrix.h>
#include <sstream>

dlib::matrix<double> read_signal_from_file(const std::string &filename);

dlib::matrix<double> get_eeg_data(std::string filename="");

dlib::matrix<double> get_ir_data(std::string filename="");

dlib::matrix<double> get_python_features();

dlib::matrix<double> get_online_python_features();


#endif
