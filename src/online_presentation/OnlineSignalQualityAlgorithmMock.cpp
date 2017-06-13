#include "OnlineSignalQualityAlgorithmMock.h"

void OnlineSignalQualityAlgorithmMock::process_input(const INeuroonSignals &input) {
  if (!m_active) {
    return;
  }

  auto &eeg = input.eeg_signal();
  auto last = eeg.size();

  // every quality measure will assess 5 eeg samples
  const int quality_sample_window = 5;
  auto i = m_last_sample_index;
  unsigned int res_count = last / quality_sample_window;

  ncSignalQuality res[res_count];

  for (; i < res_count; i++) {
    double mean = 0;
    for (int j = 0; j < quality_sample_window; j++) {
      mean += std::abs(eeg[i * quality_sample_window + j]);
    }
    mean /= quality_sample_window;

    if (mean < 350) {
      res[i] = VERY_GOOD;
    } else if (mean < 600) {
      res[i] = GOOD;
    } else if (mean < 1200) {
      res[i] = BAD;
    } else if (mean < 1200) {
      res[i] = VERY_BAD;
    } else {
      res[i] = NO_SIGNAL;
    }
  }
  m_last_sample_index = i * quality_sample_window;

  auto res_sp = std::make_shared<OnlineSignalQualityResult>(OnlineSignalQualityResult{res,res_count});
  feed_all_sinks(res_sp);
}

void OnlineSignalQualityAlgorithmMock::reset_state(){
  m_last_sample_index = 0;
}

void OnlineSignalQualityAlgorithmMock::end_streaming(const INeuroonSignals &) {}

void OnlineSignalQualityAlgorithmMock::activate() { m_active = true; }

void OnlineSignalQualityAlgorithmMock::deactivate() { m_active = false; }
