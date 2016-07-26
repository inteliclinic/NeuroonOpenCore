#include "CsvUtils.h"
#include "Exceptions.h"
#include "StringUtils.h"
#include "InValue.h"
#include <sstream>
#include <fstream>

vector<vector<InValue> > CsvReader::read_csv_no_headers(istream& inp){
  string line;
  vector<vector<InValue> > ret;

  bool fst = true;
  while(getline(inp,line)) {
    stringstream  lineStream(line);
    string        cell;

    size_t i = 0;
    while(getline(lineStream,cell,',')) {
      auto v = InValue::parse(cell);
      if(fst){
        ret.push_back({v});
      }
      else{
        ret[i].push_back(v);
      }
      i++;
    }
    fst=false;
  }

  return ret;
}

map<string, vector<InValue> > CsvReader::read_csv_with_headers(istream& inp){
  string line;
  // read headers row
  vector<string> headers;
  if(getline(inp,line)){
    string        cell;
    stringstream  lineStream(line);
    while(getline(lineStream,cell,',')){
      headers.push_back(trim(cell));
    }
  }
  // create returned map
  map<string, vector<InValue> > ret;
  for(auto s : headers){
    ret[s] = vector<InValue>();
  }

  // first line would be parsed slower to get types right
  while(getline(inp,line)) {
    stringstream  lineStream(line);
    string        cell;

    size_t i = 0;
    while(getline(lineStream,cell,',')) {
      auto v = InValue::parse(cell);
      ret[headers[i]].push_back(v);
      i++;
    }
  }

  return ret;
}

dlib::matrix<double> CsvReader::read_csv_double_matrix(istream& in, vector<string>* out_headers){

  // read headers row
  if(out_headers != nullptr){
    string line;
    if(getline(in,line)){
      string        cell;
      stringstream  lineStream(line);
      while(getline(lineStream,cell,',')){
        out_headers->push_back(trim(cell));
      }
    }

  }
  dlib::matrix<double> data;
  if (in.good()) {
    in >> data;
    if (in.fail()) {
      throw "fail!";
    }
  }
  else {
    throw "todo";
  }
  return data;
}

map<string, vector<InValue> > CsvReader::read_csv_with_headers(const string& inp){
  auto ss = stringstream(inp);
  return read_csv_with_headers(ss);
}

vector<vector<InValue> > CsvReader::read_csv_no_headers(const string& inp){
  auto ss = stringstream(inp);
  return read_csv_no_headers(ss);
}

dlib::matrix<double> CsvReader::read_csv_double_matrix(const string& inp, vector<string>* out_headers){
  auto ss = stringstream(inp);
  return read_csv_double_matrix(ss, out_headers);
}

map<string, vector<InValue> > CsvReader::read_csv_with_headers_from_path(const string& path){
  auto ifs = ifstream(path);
  return read_csv_with_headers(ifs);
}

vector<vector<InValue> > CsvReader::read_csv_no_headers_from_path(const string& path){
  auto ifs = ifstream(path);
  return read_csv_no_headers(ifs);
}

dlib::matrix<double> CsvReader::read_csv_double_matrix_from_path(const string& path, vector<string>* out_headers){
  auto ifs = ifstream(path);
  return read_csv_double_matrix(ifs, out_headers);
}
