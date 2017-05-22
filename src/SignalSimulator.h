#ifdef DESKTOP_BUILD

#ifndef __SIGNAL_SIMULATOR__
#define __SIGNAL_SIMULATOR__

#include "CommonTypes.h"
#include "CsvUtils.h"
#include "DataSink.h"
#include "FrameStreamPipe.h"
#include "SignalSource.h"
#include "logger.h"

#include <map>
#include <set>
#include <vector>

class EegFramesSource : public IPullBasedOfflineSourceSp<EegFrame> {

  std::size_t _current = 0;
  friend class SignalSource<std::int16_t>;

  std::vector<std::shared_ptr<EegFrame>> _frames = {};
  std::size_t _frame_size;

  void _check_and_parse_csv(std::string path);

public:
  EegFramesSource(std::string csv_path, std::string csv_header,
                  std::size_t frame_size = EegFrame::Length)
      : EegFramesSource(
            SignalSource<std::int16_t>::csv_column(csv_path, csv_header),
            frame_size) {}

  EegFramesSource(std::string csv_path, std::size_t csv_column_index,
                  std::size_t frame_size = EegFrame::Length)
      : EegFramesSource(
            SignalSource<std::int16_t>::csv_column(csv_path, csv_column_index),
            frame_size) {}

  EegFramesSource(SignalSource<std::int16_t> signal_source,
                  std::size_t frame_size = EegFrame::Length);
  EegFramesSource(EegFramesSource &&other)
      : _frames(std::move(other._frames)), _frame_size(other._frame_size) {}
  // ~EegFramesSource () {}

  VectorView<std::shared_ptr<EegFrame>> getValues(std::size_t count = 0) override {
    if(count == 0){
      count = count = _frames.size();
    }
    count = std::min(count,_frames.size()-_current);
    auto ret = VectorView<std::shared_ptr<EegFrame>>(_frames.begin()+_current, _frames.begin() + _current + count);
    _current = _frames.size();
    return ret;
  }

  void reset() override { _current = 0; }
  bool isDepleted() const override { return _current >= _frames.size(); }

  std::shared_ptr<EegFrame> getNextValue() override {
    return !this->isDepleted() ? _frames[_current++] : nullptr;
  }
};

class PatFramesSource : public IPullBasedOfflineSourceSp<PatFrame> {

  std::size_t _current = 0;
  friend class SignalSource<std::int16_t>;
  friend class SignalSource<std::int32_t>;
  friend class SignalSource<std::int8_t>;
  std::vector<std::shared_ptr<PatFrame>> _frames = {};

public:
  std::size_t _frame_size;

  // low level constructor
  PatFramesSource(SignalSource<std::int32_t> ir_led,
                  SignalSource<std::int32_t> red_led,
                  SignalSource<std::int16_t> accel_axes_x,
                  SignalSource<std::int16_t> accel_axes_y,
                  SignalSource<std::int16_t> accel_axes_z,
                  SignalSource<std::int8_t> temperature_1,
                  SignalSource<std::int8_t> temperature_2);

  // only ir_led in resulting frames, rest is zeros
  PatFramesSource(SignalSource<std::int32_t> ir_led)
      : PatFramesSource(ir_led, SignalSource<std::int32_t>::zeros(1),
                        SignalSource<std::int16_t>::zeros(1),
                        SignalSource<std::int16_t>::zeros(1),
                        SignalSource<std::int16_t>::zeros(1),
                        SignalSource<std::int8_t>::zeros(1),
                        SignalSource<std::int8_t>::zeros(1)) {}

  VectorView<std::shared_ptr<PatFrame>> getValues(std::size_t count = 0) override {
    if (count == 0 || count >= _frames.size())
      return VectorView<std::shared_ptr<PatFrame>>(_frames);
    return VectorView<std::shared_ptr<PatFrame>>(_frames.begin(), _frames.begin() + count);
  }

  void reset() override { _current = 0; }
  bool isDepleted() const override { return _current < _frames.size(); }

  std::shared_ptr<PatFrame> getNextValue() override {
    return !this->isDepleted() ? _frames[_current++] : nullptr;
  }
};

class SignalSimulator {

  ullong _starting_timestamp = 0;
  ullong _time_passed = 0;

  std::vector<std::tuple<uint, std::unique_ptr<IFrameStreamPipe>>> _pipes = {};

  void _set_timestamp_to_now();

public:
  SignalSimulator();
  SignalSimulator(ullong starting_timestamp)
      : _starting_timestamp(starting_timestamp) {}

  SignalSimulator(SignalSimulator &&other)
      : _starting_timestamp(other._starting_timestamp),
        _time_passed(other._time_passed), _pipes(std::move(other._pipes)) {}

  SignalSimulator(const SignalSimulator &) = delete;
  SignalSimulator &operator=(const SignalSimulator &) = delete;

  // void add_streaming_pipe(std::unique_ptr<IFrameStreamPipe> && pipe,
  //                         uint pipe_frame_emission_interval_ms){
  //   _pipes.push_back(std::make_tuple(pipe_frame_emission_interval_ms,
  //   std::move(pipe)));
  // }

  void add_streaming_pipe(std::unique_ptr<IFrameStreamPipe> pipe,
                          uint pipe_frame_emission_interval_ms);

  /** this function simulate passage of time and emits frame of data according
   *   to provided data frame sinks.
   *   @param ms_to_simulate this many ms will be simulated.
   *                         Pass 0 for emiting entire data.
   *   @param time_passing_modifier rate at which real time passes.
   *                                 0 - passing take not real time
   *                                 1 - passing take normal time
   *   @param force_wait waits ms_to_simulate * time_passing_modifier, even if
   * all pipes are exhausted.
   *
   *   @return Returns false, if all pipes are already exhausted.
   *
   *   @warning This function may actually emit the data a bit slower than
   * expected
   */
  bool pass_time(ullong ms_to_simulate, double time_passing_modifier = 1.0,
                 bool force_wait = false);
};

#endif

#endif
