/*
 * OnlineSignalQualityAlgorithmMock.h
 *
 *  Created on: Jun 12, 2017
 *  Author: Michał Adamczyk <m.adamczyk@inteliclinic.com>
 */

#ifndef SRC_OnlineSignalQualityAlgorithmMock_H_
#define SRC_OnlineSignalQualityAlgorithmMock_H_

#include "CommonTypes.h"
#include "StreamingAlgorithm.h"
#include <stdexcept>
#include <vector>

#include "NeuroonSignalStreamApi.h"

/**
 *  [TODO]
 */
struct OnlineSignalQualityResult {
  ncSignalQuality *results;
  unsigned int count;
};

/**
 * An implementation of a SinkStreamingAlgorithmMock for the online signal
 * quality mock
 */
class OnlineSignalQualityAlgorithmMock
    : public SinkStreamingAlgorithmSp<OnlineSignalQualityResult> {

  unsigned int m_last_sample_index;
  bool m_active;

public:
  using sink_t = IDataSinkSp<OnlineSignalQualityResult>;

  OnlineSignalQualityAlgorithmMock(const std::vector<sink_t *> &sinks) :
    SinkStreamingAlgorithmSp<OnlineSignalQualityResult>(sinks),
    m_last_sample_index(0),
    m_active(false){}

  virtual ~OnlineSignalQualityAlgorithmMock() {}

  virtual void reset_state() override;
  virtual void process_input(const INeuroonSignals &input) override;
  virtual void end_streaming(const INeuroonSignals &input) override;

  void activate();
  void deactivate();
};

#endif /* SRC_SLEEP_STAGING_OnlineSignalQualityAlgorithmMock_H_ */
