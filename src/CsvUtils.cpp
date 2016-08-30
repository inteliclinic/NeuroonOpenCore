#include "CsvUtils.h"
#include "Exceptions.h"
#include "StringUtils.h"
#include "InValue.h"
#include "logger.h"
#include <sstream>
#include <fstream>

std::vector<std::vector<InValue> > CsvReader::read_csv_no_headers(std::istream& inp){
  std::string line;
  std::vector<std::vector<InValue> > ret;

  bool fst = true;
  while(getline(inp,line)) {
    std::stringstream  lineStream(line);
    std::string        cell;

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

std::map<std::string, std::vector<InValue> > CsvReader::read_csv_with_headers(std::istream& inp){
  std::string line;
  // read headers row
  std::vector<std::string> headers;
  if(getline(inp,line)){
    std::string        cell;
    std::stringstream  lineStream(line);
    while(getline(lineStream,cell,',')){
      headers.push_back(trim(cell));
    }
  }
  // create returned std::map
  std::map<std::string, std::vector<InValue> > ret;
  for(auto s : headers){
    ret[s] = std::vector<InValue>();
  }

  // first line would be parsed slower to get types right
  while(getline(inp,line)) {
    std::stringstream  lineStream(line);
    std::string        cell;

    size_t i = 0;
    while(getline(lineStream,cell,',')) {
      auto v = InValue::parse(cell);
      ret[headers[i]].push_back(v);
      i++;
    }
  }

  return ret;
}

dlib::matrix<double> CsvReader::read_csv_double_matrix(std::istream& in, std::vector<std::string>* out_headers){

  // read headers row
  if(out_headers != nullptr){
    std::string line;
    if(getline(in,line)){
      std::string        cell;
      std::stringstream  lineStream(line);
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

std::map<std::string, std::vector<InValue> > CsvReader::read_csv_with_headers(const std::string& inp){
  std::stringstream ss(inp);
  return read_csv_with_headers(ss);
}

std::vector<std::vector<InValue> > CsvReader::read_csv_no_headers(const std::string& inp){
  std::stringstream ss(inp);
  return read_csv_no_headers(ss);
}

dlib::matrix<double> CsvReader::read_csv_double_matrix(const std::string& inp, std::vector<std::string>* out_headers){
  std::stringstream ss(inp);
  return read_csv_double_matrix(ss, out_headers);
}

std::map<std::string, std::vector<InValue> > CsvReader::read_csv_with_headers_from_path(const std::string& path){
  std::ifstream ifs (path);
  if(ifs.fail()){
    LOG(ERROR) << "Failed opening file.";
    throw std::runtime_error("Failed opening file.");
  }
  return read_csv_with_headers(ifs);
}

std::vector<std::vector<InValue> > CsvReader::read_csv_no_headers_from_path(const std::string& path){
  std::ifstream ifs(path);
  if(ifs.fail()){
    LOG(ERROR) << "Failed opening file.";
    throw std::runtime_error("Failed opening file.");
  }
  return read_csv_no_headers(ifs);
}

dlib::matrix<double> CsvReader::read_csv_double_matrix_from_path(const std::string& path, std::vector<std::string>* out_headers){
  std::ifstream ifs(path);
  if(ifs.fail()){
    LOG(ERROR) << "Failed opening file.";
    throw std::runtime_error("Failed opening file.");
  }
  return read_csv_double_matrix(ifs, out_headers);
}
