#include <iostream>
#include <string>
#include <istream>
#include <map>
#include <vector>
#include <string>
#include "../3rd_party/dlib/dlib/matrix.h"
#include "InValue.h"

using namespace std;

class CsvReader{

private:
  CsvReader(){}

public:

  static map<string, vector<InValue> > read_csv_with_headers(istream& inp);
  static vector<vector<InValue> > read_csv_no_headers(istream& inp);
  static dlib::matrix<double> read_csv_double_matrix(istream& inp, vector<string>* out_headers=nullptr);

  static map<string, vector<InValue> > read_csv_with_headers(const string& inp);
  static vector<vector<InValue> > read_csv_no_headers(const string& inp);
  static dlib::matrix<double> read_csv_double_matrix(const string& inp,
                                                     vector<string>* out_headers=nullptr);

  static map<string, vector<InValue> > read_csv_with_headers_from_path(const string& path);
  static vector<vector<InValue> > read_csv_no_headers_from_path(const string& path);
  static dlib::matrix<double> read_csv_double_matrix_from_path(const string& path,
                                                               vector<string>* out_headers=nullptr);

};
