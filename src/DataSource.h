#ifndef __DATA_SOURCE__
#define __DATA_SOURCE__

#include <vector>
#include <string>
#include <functional>

#include "CsvUtils.h"
#include "CommonTypes.h"
#include "logger.h"
#include "InValue.h"

template<class T>
class IPullBasedOfflineSource{

 public:
  virtual std::vector<T> get_values(std::size_t count = 0) = 0;

};


struct number_tag {}; // TODO how to make these private ?
struct string_tag {};


template < typename T > struct invalue_dispatch_type_tag;
template < > struct invalue_dispatch_type_tag<string> { typedef string_tag type; };
template < > struct invalue_dispatch_type_tag<double> { typedef number_tag type; };
template < > struct invalue_dispatch_type_tag<llong> { typedef number_tag type; };
template < > struct invalue_dispatch_type_tag<ullong> { typedef number_tag type; };
template < > struct invalue_dispatch_type_tag<std::uint32_t> { typedef number_tag type; };
template < > struct invalue_dispatch_type_tag<std::uint16_t> { typedef number_tag type; };
template < > struct invalue_dispatch_type_tag<std::uint8_t> { typedef number_tag type; };
template < > struct invalue_dispatch_type_tag<std::int32_t> { typedef number_tag type; };
template < > struct invalue_dispatch_type_tag<std::int16_t> { typedef number_tag type; };
template < > struct invalue_dispatch_type_tag<std::int8_t> { typedef number_tag type; };

template<typename T>
class SignalSource : public IPullBasedOfflineSource<T>{


  typedef std::function<T (ulong)> GenFun;
  enum class SourceOption { CSV, ZEROS, GEN_FUN };

  std::string _csv_path;
  std::string _header;
  int _column_index;
  SourceOption _option;
  GenFun _gen_fun;
  std::size_t _default_size;


  SignalSource(std::string csv_path, std::string header, int column_index, SourceOption option, GenFun gen_fun=nullptr, std::size_t samples_count=0) :
    _csv_path(csv_path), _header(header), _column_index(column_index), _option(option), _gen_fun(gen_fun), _default_size(samples_count) {}


  // T dispatch(number_tag, const InValue & val) const;
  // T dispatch(string_tag, const InValue & val) const;
  // const std::vector<T> get_values_internal() const;

public:

  static SignalSource csv_column(std::string csv_path, std::string header){
    return SignalSource(csv_path, header, -1, SourceOption::CSV);
  }

  static SignalSource csv_column(std::string csv_path, int column_index){
    return SignalSource(csv_path, "", column_index, SourceOption::CSV);
  }

  static SignalSource zeros(std::size_t default_samples_count){
    return SignalSource("", "", -1, SourceOption::ZEROS, nullptr, default_samples_count);
 }

  static SignalSource generation_from_index(std::function<T (ulong)> gen_fun, std::size_t default_samples_count){
    return SignalSource("", "", -1, SourceOption::GEN_FUN, gen_fun, default_samples_count);
  }

  const std::vector<T> get_values(std::size_t count=0) const{
    return get_values_internal(count);
  }

  std::vector<T> get_values(std::size_t count=0) override{
    return get_values_internal(count);
  }

  static std::vector< std::vector<T> > get_values_from_same_csv(const std::vector<SignalSource<T>> same_csv_sources){

    if(same_csv_sources.empty()){
      LOG(WARNING) << "Passed empty sources vector.";
      return {};
    }

    auto fname = same_csv_sources[0]._csv_path;
    for(const auto & ss : same_csv_sources){
      if(ss._option != SignalSource::Option::CSV){
        LOG(ERROR) << "Vector contains non-csv source.";
        throw std::logic_error("Pass only csv source.");
      }
      if(ss._csv_path != fname){
        LOG(ERROR) << "Vector contains csv sources with different file paths.";
        throw std::logic_error("The source uses different csv file.");
      }

    }

    // TODO - make it on one read of csv

    std::vector< std::vector<T> > ret = {};
    for(const auto & ss : same_csv_sources){
      ret.push_back(ss.get_values());
    }
    return ret;
  }


  T dispatch(number_tag, const InValue & val) const{
    switch(val.type()){
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

  T dispatch(string_tag, const InValue & val) const{
    switch(val.type()){
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

  const std::vector<T> get_values_internal(std::size_t count=0) const{
    typedef typename invalue_dispatch_type_tag<T>::type tag;


    std::vector<InValue> invalues = {};
    switch(_option){
    case SourceOption::GEN_FUN:{
      if(count == 0){
        count = _default_size;
      }
      std::vector<T> rets = {};
      for(int i = 0; i < count; i++){
        rets.push_back(_gen_fun(i));
      }
      return rets;
    }
      break;
    case SourceOption::ZEROS:
      if(count == 0){
        count = _default_size;
      }
      for(int i = 0; i < count; i++){
        invalues.push_back(InValue((llong)0));
      }
      break;
    case SourceOption::CSV:{
      if(_header!=""){
        invalues = CsvReader::read_csv_with_headers_from_path(_csv_path).at(_header);
      }
      else if(_column_index>=0){
        invalues = CsvReader::read_csv_no_headers_from_path(_csv_path)[_column_index];
      }
      else{
        throw std::logic_error("Invalid SignalSource initialization");
      }
      if(count == 0){
        count = invalues.size();
      }
    }
      break;
    }

    std::vector<T> rets = {};
    if(count >= invalues.size()){ count = invalues.size();}
    for(std::size_t i=0; i<count; i++){
      rets.push_back(dispatch(tag(), invalues[i]));
    }
    return rets;
  }


};

#endif
