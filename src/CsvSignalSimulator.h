#ifndef __CSV_SIGNAL_SIMULATOR__
#define __CSV_SIGNAL_SIMULATOR__

#include "CommonTypedefs.h"
#include <vector>
#include <set>
#include <map>
#include "DataSink.h"
#include "CsvUtils.h"
// #include "NeuroonSignals.h"

// class providing access to vector of timestamped values
// with indication on frame generation frequency
// and spec of the signal.

class CsvSignalSource{

  SignalSpec _spec;
  std::vector<int> _signal;
  int _ms_per_frame;

  void check_and_parse_csv(std::string);
public:

  // path should point to csv value with timestamped values
  // and header containing fields: "timestamp" and "signal"
  CsvSignalSource(std::string path, SignalSpec spec, int ms_per_frame);

  CsvSignalSource(const CsvSignalSource & rhs) :
    _spec(SignalSpec(rhs._spec)), _signal(std::vector<int>(rhs._signal)){}

  CsvSignalSource(CsvSignalSource && rhs) :
    _spec(std::move(rhs._spec)), _signal(std::move(rhs._signal)){}

  SignalSpec spec() const { return _spec;}
  const std::vector<int> & signal() const { return _signal;}
  int ms_per_frame() const { return _ms_per_frame; }

  // void init_from_

  static CsvSignalSource eeg_signal_source(std::string path, int ms_per_frame=60);
  static CsvSignalSource irled_signal_source(std::string path, int ms_per_frame=150);
};


class CsvSignalSimulator{

  ullong _time_passed = 0;
  std::map<CsvSignalSource*, std::pair<int, std::size_t>> _signal_bookmarks = {};

public:

  std::vector<CsvSignalSource*> sources = {};
  std::vector<SignalFrameDataSink*> sinks = {};


  void reset();
  void pass_time(ullong ms_to_simulate,
                 double time_passing_modifier=1.0);






};

#endif
