#include "DataSource.h"
#include "CsvUtils.h"
#include <string>

template < typename T > struct invalue_dispatch_type_tag;
template < > struct invalue_dispatch_type_tag<string> { typedef string_tag type; };
template < > struct invalue_dispatch_type_tag<double> { typedef number_tag type; };
template < > struct invalue_dispatch_type_tag<int> { typedef number_tag type; };

template<typename T>
T SignalSource<T>::dispatch(number_tag, const InValue & val) const{
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

template<typename T>
T SignalSource<T>::dispatch(string_tag, const InValue & val) const{
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

template<typename T>
const std::vector<T> SignalSource<T>::get_values_internal() const{
  typedef typename invalue_dispatch_type_tag<T>::type tag;

  std::vector<InValue> invalues = {};
  switch(_option){
  case SourceOption::ZEROS:
    for(int i = 0; i < _size; i++){
      invalues.push_back(InValue((llong)0));
    }
    break;
  case SourceOption::GEN_FUN:
    for(int i = 0; i < _size; i++){
      invalues.push_back(_gen_fun(i));
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
  }
    break;
  }

  std::vector<T> rets = {};
  for(const auto & iv : invalues){
    rets.push_back(dispatch(tag(), iv));
  }
  return rets;
}



// static SignalSource csv_column(std::string csv_path, std::string header){
//   return SignalSource(csv_path, header, -1, SourceOption::CSV);
// }

//   static SignalSource csv_column(std::string csv_path, int column_index){
//     return SignalSource(csv_path, "", column_index, SourceOption::CSV);
//   }

//   static SignalSource zeros(std::size_t count){
//     return SignalSource("", "", -1, SourceOption::ZEROS, nullptr, count);
//   }

//   static SignalSource generation_from_index(std::function<InValue (ulong)> gen_fun, std::size_t samples_count){
//     return SignalSource("", "", -1, SourceOption::GEN_FUN, gen_fun, samples_count);
//   }

//   // const std::vector<InValue> get_values() const{
//   //   return get_values_internal();
//   // }

//   // std::vector<InValue> get_values() override{
//   //   return get_values_internal();
//   // }

//   static std::vector< std::vector<InValue> > get_values_from_same_csv(const std::vector<SignalSource> same_csv_sources);
