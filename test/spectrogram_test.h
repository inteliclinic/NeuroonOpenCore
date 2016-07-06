#include <vector>
#include <iostream>

void test_spectrogram() {
	const int size = 2048;
	const double omega = 0.1;
	std::vector<double> data(size);
	std::vector<double> original(size);

	for (std::size_t i = 0; i != data.size(); ++i) {
		data[i] = sin(M_PI * omega * i);
		original[i] = sin(M_PI * omega * i);
	}

	std::vector<std::vector<double>> res = spectrogram(data, 128);
	for (int i = 0; i != res.size(); ++i) {

		for (int j = 0; j != res[i].size(); ++j) {
			std::cout << res[i][j];
			if (j != res[i].size() -1) {
				 std::cout << ',';
			}
		}
		std::cout << std::endl;
	}
}
