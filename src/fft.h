#ifndef FFT_H
#define FFT_H

#include <vector>

std::vector<double> fft(const std::vector<double>& data, int begin=0, int end=-1);

std::vector<std::vector<double>> spectrogram(const std::vector<double>& signal, int window);

#endif
