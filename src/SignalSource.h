#ifndef __NC_SIGNAL_SOURCE__
#define __NC_SIGNAL_SOURCE__

#include "CommonTypes.h"
#include "CsvUtils.h"
#include "DataSource.h"
#include "VectorView.h"
#include "logger.h"
#include <string>

struct number_tag {}; // TODO how to make these private ?
struct string_tag {};

template <typename T> struct invalue_dispatch_type_tag;
template <> struct invalue_dispatch_type_tag<std::string> {
  typedef string_tag type;
};
template <> struct invalue_dispatch_type_tag<double> {
  typedef number_tag type;
};
template <> struct invalue_dispatch_type_tag<llong> {
  typedef number_tag type;
};
template <> struct invalue_dispatch_type_tag<ullong> {
  typedef number_tag type;
};
template <> struct invalue_dispatch_type_tag<std::uint32_t> {
  typedef number_tag type;
};
template <> struct invalue_dispatch_type_tag<std::uint16_t> {
  typedef number_tag type;
};
template <> struct invalue_dispatch_type_tag<std::uint8_t> {
  typedef number_tag type;
};
template <> struct invalue_dispatch_type_tag<std::int32_t> {
  typedef number_tag type;
};
template <> struct invalue_dispatch_type_tag<std::int16_t> {
  typedef number_tag type;
};
template <> struct invalue_dispatch_type_tag<std::int8_t> {
  typedef number_tag type;
};

class SourceSpec {
public:
  enum class SourceOption { CSV, ZEROS, GEN_FUN };

private:
  std::string _csv_path;
  std::string _header;
  int _column_index;
  SourceOption _option;
  std::size_t _default_size;

public:
  SourceSpec(SourceOption option, std::string csv_path, std::string header,
             int column_index, std::size_t default_size = 0)
      : _csv_path(csv_path), _header(header), _column_index(column_index),
        _option(option), _default_size(default_size) {}

  SourceOption option() const { return _option; }
  std::size_t default_size() const { return _default_size; }
  std::string csv_path() const {
    return (option() == SourceOption::CSV) ? _csv_path : "";
  }
  std::string csv_header() const {
    return (option() == SourceOption::CSV) ? _header : "";
  }
  int csv_column_index() const {
    return (option() == SourceOption::CSV) ? _column_index : -1;
  }
};

template <typename T> class SignalSource : public IPullBasedOfflineSource<T> {

  std::size_t _current = 0;

  typedef std::function<T(ulong)> GenFun;
  SourceSpec _spec;
  GenFun _gen_fun;
  std::vector<T> _data = {};

  SignalSource(SourceSpec spec, GenFun gen_fun = nullptr)
      : _spec(spec), _gen_fun(gen_fun) {
    auto vv = this->getValues();
    _data = std::vector<T>(vv.begin(), vv.end());
  }

  // T dispatch(number_tag, const InValue & val) const;
  // T dispatch(string_tag, const InValue & val) const;
  // const std::vector<T> getValuesInternal() const;

public:
  SourceSpec spec() const { return _spec; }

  static SignalSource csv_column(std::string csv_path, std::string header) {
    return SignalSource(
        SourceSpec(SourceSpec::SourceOption::CSV, csv_path, header, -1));
  }

  static SignalSource csv_column(std::string csv_path, int column_index) {
    return SignalSource(
        SourceSpec(SourceSpec::SourceOption::CSV, csv_path, "", column_index));
  }

  static SignalSource zeros(std::size_t default_samples_count) {
    return SignalSource(SourceSpec(SourceSpec::SourceOption::ZEROS, "", "", -1,
                                   default_samples_count));
  }

  static SignalSource generation_from_index(std::function<T(ulong)> gen_fun,
                                            std::size_t default_samples_count) {
    return SignalSource(SourceSpec(SourceSpec::SourceOption::GEN_FUN, "", "",
                                   -1, default_samples_count),
                        gen_fun);
  }

  void reset() override { _current = 0; }
  bool isDepleted() const override { return _current < _data.size(); }
  T getNextValue() override {
    if (!this->isDepleted()) {
      _data[_current++];
    }
    throw std::logic_error(
        "Source is depleted. If you want to use it again, call reset.");
  }

  VectorView<T> getValues(std::size_t count = 0) override {
    return getValuesInternal(count);
  }

  static std::vector<std::vector<T>>
  getValues_from_same_csv(const std::vector<SignalSource<T>> same_csv_sources) {

    if (same_csv_sources.empty()) {
      LOG(WARNING) << "Passed empty sources vector.";
      return {};
    }

    auto fname = same_csv_sources[0]._csv_path;
    for (const auto &ss : same_csv_sources) {
      if (ss._option != SignalSource::Option::CSV) {
        LOG(ERROR) << "Vector contains non-csv source.";
        throw std::logic_error("Pass only csv source.");
      }
      if (ss._csv_path != fname) {
        LOG(ERROR) << "Vector contains csv sources with different file paths.";
        throw std::logic_error("The source uses different csv file.");
      }
    }

    // TODO - make it on one read of csv
    std::vector<std::vector<T>> ret = {};
    for (const auto &ss : same_csv_sources) {
      ret.push_back(ss.getValues());
    }
    return ret;
  }

  T dispatch(number_tag, const InValue &val) const {
    switch (val.type()) {
    case InValue::Type::DOUBLE:
      return (T)val.double_value();
      break;
    case InValue::Type::LLONG:
      return (T)val.llong_value();
      break;
    case InValue::Type::STRING:
      throw std::bad_cast();
    }
  }

  T dispatch(string_tag, const InValue &val) const {
    switch (val.type()) {
    case InValue::Type::DOUBLE:
      return (T)std::to_string(val.double_value());
      break;
    case InValue::Type::LLONG:
      return (T)std::to_string(val.llong_value());
      break;
    case InValue::Type::STRING:
      return (T)val.string_value();
    }
  }

  VectorView<T> getValuesInternal(std::size_t count = 0, bool clear = false) {
    if (!clear && _data.size()) {
      auto b = std::min(_data.begin()+_current, _data.end());
      return VectorView<T>(b, _data.end());
    }
    _data.clear();
    typedef typename invalue_dispatch_type_tag<T>::type tag;

    std::vector<InValue> invalues = {};

    switch (_spec.option()) {
    case SourceSpec::SourceOption::GEN_FUN: {
      if (count == 0) {
        count = _spec.default_size();
      }
      for (std::size_t i = 0; i < count; i++) {
        _data.push_back(_gen_fun(i));
      }
      return VectorView<T>(_data.begin(), _data.end());
    } break;
    case SourceSpec::SourceOption::ZEROS:
      if (count == 0) {
        count = _spec.default_size();
      }
      for (int i = 0; i < count; i++) {
        invalues.push_back(InValue((llong)0));
      }
      break;
    case SourceSpec::SourceOption::CSV: {
      if (_spec.csv_header() != "") {
        invalues = CsvReader::read_csv_with_headers_from_path(_spec.csv_path())
                       .at(_spec.csv_header());
      } else if (_spec.csv_column_index() >= 0) {
        invalues = CsvReader::read_csv_no_headers_from_path(
            _spec.csv_path())[_spec.csv_column_index()];
      } else {
        throw std::logic_error("Invalid SignalSource initialization");
      }
      if (count == 0) {
        count = invalues.size();
      }
    } break;
    }

    if (count >= invalues.size()) {
      count = invalues.size();
    }
    for (std::size_t i = 0; i < count; i++) {
      _data.push_back(dispatch(tag(), invalues[i]));
    }
    return VectorView<T>(_data.begin(), _data.end());
  }
};

template <class T> using SignalSourceSp = SignalSource<std::shared_ptr<T>>;

#endif
