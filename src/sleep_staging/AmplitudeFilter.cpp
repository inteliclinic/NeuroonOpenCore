/*
 * AmplitudeFilter.cpp
 *
 *  Created on: Aug 3, 2016
 *      Author: tomek
 */

#include "AmplitudeFilter.h"
#include <cmath>
#include <vector>
#include <cassert>
#include "logger.h"
#include "dlib_utils.h"

AmplitudeFilter::AmplitudeFilter(double critical_value)
	: m_critical_value(critical_value) {}

dlib::matrix<double> AmplitudeFilter::transform(const dlib::matrix<double> &data, const dlib::matrix<double> &filter_column) {
	if (data.nr() != filter_column.nr()) {
		throw std::logic_error("Incorrect filter!");
	}

	LOG(WARNING) << "filter: " << filter_column(0,0);

	dlib::matrix<int> incorrect_rows = rows_greater_than(filter_column, m_critical_value);
	dlib::matrix<double> result = data;

	if (incorrect_rows.size() > 0) {
		dlib::matrix<double> incorrect_data(incorrect_rows.size(), data.nc());
		dlib::set_all_elements(incorrect_data, NAN);
		dlib::set_rowm(result, incorrect_rows) = incorrect_data;
	}
	return result;
}

