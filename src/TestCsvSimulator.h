#ifndef __TEST_CSV_SIMULATOR__
#define __TEST_CSV_SIMULATOR__

#include "SignalSimulator.h"

class TestCsvSignalSimulator : public SignalSimulator{


  SignalSimulator _sim;

  std::shared_ptr<IPullBasedOfflineSource<EegFrame>> _eeg_source_sp;
  std::shared_ptr<IPullBasedOfflineSource<AccelLedsTempFrame>> _alt_source_sp;
  std::shared_ptr<IDataSink<EegFrame>> _eeg_sink;
  std::shared_ptr<IDataSink<AccelLedsTempFrame>> _alt_sink;

public:

  TestCsvSignalSimulator(std::string eeg_csv_path, std::string irled_csv_path,
                         std::shared_ptr<IDataSink<EegFrame>> eeg_sink,
                         std::shared_ptr<IDataSink<AccelLedsTempFrame>> alt_sink){

    _alt_source_sp = std::shared_ptr<IPullBasedOfflineSource<AccelLedsTempFrameSource>>(
       new AccelLedsTempFrameSource(SignalSource<std::int32_t>::csv_column(irled_csv_path, "signal")));

    _eeg_source_sp = std::shared_ptr<IPullBasedOfflineSource<EegFrame>>(new EegFramesSource(eeg_csv_path, EegFrame::Length));

    _eeg_sink = eeg_sink;
    _alt_sink = alt_sink;

    auto pipe1_up = std::unique_ptr<IFrameStreamPipe>(new FrameStreamPipe<EegFrame>(_eeg_source_sp, _eeg_sink));
    auto pipe2_up = std::unique_ptr<IFrameStreamPipe>(new FrameStreamPipe<AccelLedsTempFrame>(_alt_source_sp, _alt_sink));

    _sim.add_streaming_pipe(std::move(pipe1_up), EegFrame::DefaultEmissionInterval_ms);
    _sim.add_streaming_pipe(std::move(pipe2_up), EegFrame::DefaultEmissionInterval_ms);
  }


  TestCsvSignalSimulator(std::string eeg_csv_path, std::string irled_csv_path,
                         std::function<void (EegFrame&)> consume_eeg_function,
                         std::function<void (AccelLedsTempFrame&)> consume_accelledstemp_function){

    _alt_source_sp = std::shared_ptr<IPullBasedOfflineSource<AccelLedsTempFrameSource>>(
       new AccelLedsTempFrameSource(SignalSource<std::int32_t>::csv_column(irled_csv_path, "signal")));

    _eeg_source_sp = std::shared_ptr<IPullBasedOfflineSource<EegFrame>>(new EegFramesSource(eeg_csv_path, EegFrame::Length));

    _eeg_sink = std::shared_ptr<LambdaSignalFrameDataSink<EegFrame>>(new LambdaSignalFrameDataSink<EegFrame>(consume_eeg_function));
    _alt_sink = std::shared_ptr<LambdaSignalFrameDataSink<AccelLedsTempFrame>>(new LambdaSignalFrameDataSink<AccelLedsTempFrame>(consume_accelledstemp_function));


    auto pipe1_up = std::unique_ptr<IFrameStreamPipe>(new FrameStreamPipe<EegFrame>(_eeg_source_sp, _eeg_sink));
    auto pipe2_up = std::unique_ptr<IFrameStreamPipe>(new FrameStreamPipe<AccelLedsTempFrame>(_alt_source_sp, _alt_sink));

    _sim.add_streaming_pipe(std::move(pipe1_up), EegFrame::DefaultEmissionInterval_ms);
    _sim.add_streaming_pipe(std::move(pipe2_up), EegFrame::DefaultEmissionInterval_ms);
  }

  SignalSimulator & get_simulator() { return _sim; }

};

#endif
