#ifndef __DATA_SOURCE__
#define __DATA_SOURCE__

#include <vector>
#include <string>
#include <functional>

#include "CommonTypes.h"
#include "InValue.h"

template<class T>
class IPullBasedOfflineSource{

 public:
  virtual std::vector<T> get_values() = 0;

};


// TODO how to make it private ?

struct number_tag {};
struct string_tag {};

template<typename T>
class SignalSource : public IPullBasedOfflineSource<T>{


  typedef std::function<InValue (ulong)> GenFun;
  enum class SourceOption { CSV, ZEROS, GEN_FUN };

  std::string _csv_path;
  std::string _header;
  int _column_index;
  SourceOption _option;
  GenFun _gen_fun;
  std::size_t _size;


  SignalSource(std::string csv_path, std::string header, int column_index, SourceOption option, GenFun gen_fun=nullptr, std::size_t samples_count=0) :
    _csv_path(csv_path), _header(header), _column_index(column_index), _option(option), _gen_fun(gen_fun), _size(samples_count) {}


  T dispatch(number_tag, const InValue & val) const;
  T dispatch(string_tag, const InValue & val) const;
  const std::vector<T> get_values_internal() const;

public:

  static SignalSource csv_column(std::string csv_path, std::string header){
    return SignalSource(csv_path, header, -1, SourceOption::CSV);
  }

  static SignalSource csv_column(std::string csv_path, int column_index){
    return SignalSource(csv_path, "", column_index, SourceOption::CSV);
  }

  static SignalSource zeros(std::size_t count){
    return SignalSource("", "", -1, SourceOption::ZEROS, nullptr, count);
  }

  static SignalSource generation_from_index(std::function<InValue (ulong)> gen_fun, std::size_t samples_count){
    return SignalSource("", "", -1, SourceOption::GEN_FUN, gen_fun, samples_count);
  }

  const std::vector<T> get_values() const{
    return get_values_internal();
  }

  std::vector<T> get_values() override{
    return get_values_internal();
  }

  static std::vector< std::vector<T> > get_values_from_same_csv(const std::vector<SignalSource<T>> same_csv_sources);

};


#endif

/* class SignalSource : public IPullBasedOfflineSource<InValue>{ */

/*   typedef std::function<InValue (ulong)> GenFun; */
/*   enum class SourceOption { CSV, ZEROS, GEN_FUN }; */

/*   std::string _csv_path; */
/*   std::string _header; */
/*   int _column_index; */
/*   SourceOption _option; */
/*   GenFun _gen_fun; */
/*   std::size_t _size; */


/*   SignalSource(std::string csv_path, std::string header, int column_index, SourceOption option, GenFun gen_fun=nullptr, std::size_t samples_count=0) : */
/*     _csv_path(csv_path), _header(header), _column_index(column_index), _option(option), _gen_fun(gen_fun), _size(samples_count) {} */



/*   const std::vector<InValue> get_values_internal() const; */

/* public: */

/*   static SignalSource csv_column(std::string csv_path, std::string header){ */
/*     return SignalSource(csv_path, header, -1, SourceOption::CSV); */
/*   } */

/*   static SignalSource csv_column(std::string csv_path, int column_index){ */
/*     return SignalSource(csv_path, "", column_index, SourceOption::CSV); */
/*   } */

/*   static SignalSource zeros(std::size_t count){ */
/*     return SignalSource("", "", -1, SourceOption::ZEROS, nullptr, count); */
/*   } */

/*   static SignalSource generation_from_index(std::function<InValue (ulong)> gen_fun, std::size_t samples_count){ */
/*     return SignalSource("", "", -1, SourceOption::GEN_FUN, gen_fun, samples_count); */
/*   } */

/*   const std::vector<InValue> get_values() const{ */
/*     return get_values_internal(); */
/*   } */

/*   std::vector<InValue> get_values() override{ */
/*     return get_values_internal(); */
/*   } */

/*   static std::vector< std::vector<InValue> > get_values_from_same_csv(const std::vector<SignalSource> same_csv_sources); */

/* } */;
