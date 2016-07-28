#include <iostream>
#include <string>
#include <istream>
#include <map>
#include <vector>
#include <string>
#include "../3rd_party/dlib/dlib/matrix.h"
#include "InValue.h"


class CsvReader{

private:
  CsvReader(){}

public:

  static std::map<std::string, std::vector<InValue> > read_csv_with_headers(std::istream& inp);
  static std::vector<std::vector<InValue> > read_csv_no_headers(std::istream& inp);
  static dlib::matrix<double> read_csv_double_matrix(std::istream& inp, std::vector<std::string>* out_headers=nullptr);

  static std::map<std::string, std::vector<InValue> > read_csv_with_headers(const std::string& inp);
  static std::vector<std::vector<InValue> > read_csv_no_headers(const std::string& inp);
  static dlib::matrix<double> read_csv_double_matrix(const std::string& inp,
                                                     std::vector<std::string>* out_headers=nullptr);

  static std::map<std::string, std::vector<InValue> > read_csv_with_headers_from_path(const std::string& path);
  static std::vector<std::vector<InValue> > read_csv_no_headers_from_path(const std::string& path);
  static dlib::matrix<double> read_csv_double_matrix_from_path(const std::string& path,
                                                               std::vector<std::string>* out_headers=nullptr);

};

