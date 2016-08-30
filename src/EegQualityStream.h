#ifndef __EEG_QUALITY_STREAM__
#define __EEG_QUALITY_STREAM__

#include "VectorView.h"
#include "StreamingAlgorithm.h"

enum class EegQuality{NO_SIGNAL, BAD, AVERAGE, GOOD};


class EegQualityStream : public SinkStreamingAlgorithm<EegQuality>{
private:

  int _window_size;
  int _overlap;
  std::size_t _last_counter;

  EegQuality _compute_quality(VectorView<double>&& eeg_signal);
public:

  /**
   *  @param window_size Number of samples from which the signal quality value will be computed.
   *  @param overlap This many samples will be included from previous computation.
   *  @param sinks The result from each window window will be send to each data sink.
   */
  EegQualityStream(int window_size, int overlap=0, const std::vector<IDataSink<EegQuality>*> & sinks={}) :
    SinkStreamingAlgorithm(sinks),_window_size(window_size), _overlap(overlap) {}


  virtual void reset_state() override;
  virtual void process_input(const NeuroonSignals & input) override;
  virtual void end_streaming (const NeuroonSignals & ) override {}

};

#endif
