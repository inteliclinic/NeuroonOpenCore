#ifndef __CSV_SIGNAL_SIMULATOR__
#define __CSV_SIGNAL_SIMULATOR__

#include "CommonTypes.h"
#include "logger.h"
#include "DataSink.h"
#include "CsvUtils.h"
#include "FrameStreamPipe.h"

#include <vector>
#include <set>
#include <map>

class CsvEegFramesSource : public IPullBasedFrameSource<EegFrame>{

  std::vector<EegFrame> _frames = {};
  std::size_t _frame_size;
  int _column = 0;

  void _check_and_parse_csv(std::string path);
public:

  CsvEegFramesSource (std::string path, std::size_t frame_size = EegFrame::Length, int column = 0);
  CsvEegFramesSource (CsvEegFramesSource && other) :
    _frames(std::move(other._frames)),
    _frame_size(other._frame_size) {}
  // ~CsvEegFramesSource () {}

  std::vector<EegFrame> & get_frames () override { return _frames; }
};


class CsvAccelLedsTempFrameSource : public IPullBasedFrameSource<AccelLedsTempFrame>{

  std::vector<AccelLedsTempFrame> _frames = {};
  std::size_t _frame_size;
  int _column = 0;

  void _check_and_parse_csv(std::string path);
 public:

  CsvAccelLedsTempFrameSource  (std::string path, int column = 0);
  CsvAccelLedsTempFrameSource  (CsvAccelLedsTempFrameSource  && other) :
  _frames(std::move(other._frames)),
    _frame_size(other._frame_size) {}
  // ~CsvAccelLedsTempFrameSource  () {}

  std::vector<AccelLedsTempFrame> & get_frames () override { return _frames; }
};


class CsvSignalSimulator{


  ullong _starting_timestamp = 0;
  ullong _time_passed = 0;

  std::vector<std::tuple<uint, std::unique_ptr<IFrameStreamPipe>>> _pipes = {};

  void _set_timestamp_to_now();


public:

  CsvSignalSimulator();
  CsvSignalSimulator(ullong starting_timestamp) :
    _starting_timestamp(starting_timestamp) {}


  CsvSignalSimulator(CsvSignalSimulator && other) :
    _starting_timestamp(other._starting_timestamp),
    _time_passed(other._time_passed),
    _pipes(std::move(other._pipes)) {}


  CsvSignalSimulator(const CsvSignalSimulator &) = delete;
  CsvSignalSimulator& operator=(const CsvSignalSimulator &) = delete;

  void add_streaming_pipe(std::unique_ptr<IFrameStreamPipe> & pipe,
                          uint pipe_frame_emission_interval_ms);

  void pass_time(ullong ms_to_simulate,
                 double time_passing_modifier=1.0);

};

#endif
