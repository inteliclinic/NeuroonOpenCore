#include "fft.h"
#include <vector>
#include <cmath>

#include <iostream>

void fft_test() {
	const int size = 2048;
	const double omega = 0.1;
	std::vector<double> data(size);
	std::vector<double> original(size);

	for (std::size_t i = 0; i != data.size(); ++i) {
		data[i] = sin(M_PI * omega * i);
		original[i] = sin(M_PI * omega * i);
	}

	auto res = fft(data);
	for (int i = 0; i != data.size(); ++i) {
		std::cout << data[i] <<',' << res[i] << std::endl;
	}
}
