/*
 * EntropyFilter.cpp
 *
 *  Created on: Aug 4, 2016
 *      Author: tomek
 */

#include "EntropyFilter.h"
#include <cmath>
#include "signal_utils.h"

EntropyFilter::EntropyFilter(double critical_value)
: m_critical_value(critical_value)
{
	// TODO Auto-generated constructor stub

}

EntropyFilter::~EntropyFilter() {
	// TODO Auto-generated destructor stub
}

dlib::matrix<double> EntropyFilter::transform(const dlib::matrix<double> &data) {
	dlib::matrix<double> entropies(data.nr(), 1);

	dlib::matrix<double> result(data.nr(), data.nc());
	result = data;
	for (int i = 0; i != entropies.nr(); ++i) {
		double entr = entropy(dlib::rowm(data, i));
		if (entr > m_critical_value) {
			dlib::set_rowm(result, i) = NAN;
		}
	}
	return result;
}
