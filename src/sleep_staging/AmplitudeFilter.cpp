/*
 * AmplitudeFilter.cpp
 *
 *  Created on: Aug 3, 2016
 *      Author: tomek
 */

#include "AmplitudeFilter.h"
#include <cmath>
#include <vector>

AmplitudeFilter::AmplitudeFilter(double critical_value, int column)
	: m_critical_value(critical_value)
	, m_column(column)
{
	// TODO Auto-generated constructor stub

}

AmplitudeFilter::~AmplitudeFilter() {
	// TODO Auto-generated destructor stub
}

dlib::matrix<int> AmplitudeFilter::rows_greater_than(const dlib::matrix<double> &signal, double threshold) {
	std::vector<int> rows;

	for (int i = 0; i != signal.nr(); ++i) {
		double value = signal(i, 0);
		if (value > threshold) {
			rows.push_back(i);
		}
	}

	dlib::matrix<int> result(rows.size(), 1);
	for (std::size_t i = 0; i != rows.size(); ++i) {
		result(i, 0) = rows[i];
	}

	return result;
}

dlib::matrix<double> AmplitudeFilter::transform(const dlib::matrix<double> &data) {
	dlib::matrix<int> incorrect_rows = rows_greater_than(dlib::colm(data, m_column), m_critical_value);
	dlib::matrix<double> result = data;

	if (incorrect_rows.size() > 0) {
		dlib::matrix<double> incorrect_data(incorrect_rows.size(), data.nc());
		dlib::set_all_elements(incorrect_data, NAN);
		dlib::set_rowm(result, incorrect_rows) = incorrect_data;
	}
	return result;
}

