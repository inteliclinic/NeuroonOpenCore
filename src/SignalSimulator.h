#ifndef __SIGNAL_SIMULATOR__
#define __SIGNAL_SIMULATOR__

#include "CommonTypes.h"
#include "logger.h"
#include "DataSink.h"
#include "CsvUtils.h"
#include "FrameStreamPipe.h"

#include <vector>
#include <set>
#include <map>

class EegFramesSource : public IPullBasedOfflineSource<EegFrame>{
  friend class SignalSource<std::int16_t>;

  std::vector<EegFrame> _frames = {};
  std::size_t _frame_size;

  void _check_and_parse_csv(std::string path);
public:

  EegFramesSource (std::string path, std::size_t frame_size = EegFrame::Length);
  EegFramesSource (SignalSource<std::int16_t> signal_source, std::size_t frame_size = EegFrame::Length);
  EegFramesSource (EegFramesSource && other) :
    _frames(std::move(other._frames)),
    _frame_size(other._frame_size) {}
  // ~EegFramesSource () {}

  std::vector<EegFrame> get_values () override { return _frames; }
};


class AccelLedsTempFrameSource : public IPullBasedOfflineSource<AccelLedsTempFrame>{
  friend class SignalSource<std::int16_t>;
  friend class SignalSource<std::int32_t>;
  friend class SignalSource<std::int8_t>;

public:



  // low level constructor
  AccelLedsTempFrameSource (SignalSource<std::int32_t> ir_led,
                            SignalSource<std::int32_t> red_led,
                            SignalSource<std::int16_t> accel_axes_x,
                            SignalSource<std::int16_t> accel_axes_y,
                            SignalSource<std::int16_t> accel_axes_z,
                            SignalSource<std::int8_t> temperature_1,
                            SignalSource<std::int8_t> temperature_2);

  // // irled,
  // AccelLedsTempFrameSource(SignalSource ir_led, SignalSource acc);


};


class SignalSimulator{

  ullong _starting_timestamp = 0;
  ullong _time_passed = 0;

  std::vector<std::tuple<uint, std::unique_ptr<IFrameStreamPipe>>> _pipes = {};

  void _set_timestamp_to_now();


public:

  SignalSimulator();
  SignalSimulator(ullong starting_timestamp) :
    _starting_timestamp(starting_timestamp) {}


  SignalSimulator(SignalSimulator && other) :
    _starting_timestamp(other._starting_timestamp),
    _time_passed(other._time_passed),
    _pipes(std::move(other._pipes)) {}


  SignalSimulator(const SignalSimulator &) = delete;
  SignalSimulator& operator=(const SignalSimulator &) = delete;

  void add_streaming_pipe(std::unique_ptr<IFrameStreamPipe> & pipe,
                          uint pipe_frame_emission_interval_ms);

  void pass_time(ullong ms_to_simulate,
                 double time_passing_modifier=1.0);

};

#endif
