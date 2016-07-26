#include <cstdlib>
#include "InValue.h"
#include "StringUtils.h"
#include "Exceptions.h"


int parse_integer(const string & str_inp, bool* error){
  char *ep=0;
  const char* str = str_inp.c_str();
  int ret = strtol(str, &ep, 0);
  if(ep!=str + str_inp.length()){
    if(error) *error = 1;
    return 0;
  }
  return ret;
}

double parse_double(const string & str_inp, bool* error){
  char *ep=0;
  const char* str = str_inp.c_str();
  double ret = strtod(str, &ep);
  if(ep!=str + str_inp.length()){
    if(error) *error = 1;
    return 0;
  }
  return ret;
}

InValue InValue::parse(const string & inp){
  auto str = trim(inp);
  bool error = 0;
  int r1 = parse_integer(str, &error);
  if(!error) return InValue(r1);
  error = 0;
  double r2 = parse_double(str, &error);
  if(!error) return InValue(r2);
  return InValue(str);
}

ostream& operator<<(ostream& os, const InValue& inv){
  switch(inv.type()){
  case InValue::Type::INT:
    os << inv.int_value();
    break;
  case InValue::Type::DOUBLE:
    os << inv.double_value();
    break;
  case InValue::Type::STRING:
    os << inv.string_value();
    break;
  default:
    throw NotImplemented("Did you forget to implement?");
  }
  return os;
}
