#ifndef __INVALUE__
#define __INVALUE__
#include <iostream>
#include <string>

using std::string;
typedef long long llong;

class InValue{
public:
  enum Type{DOUBLE, LLONG, STRING};

private:
  Type type_tag;

  union{
    double d;
    llong i;
    std::string s;
  };
public:

  InValue(){
    new (&s) std::string();
  }

  InValue(const InValue& o){
    type_tag = Type(o.type_tag);
    switch(type_tag){
    case Type::LLONG:
      i = o.i;
      break;
    case Type::DOUBLE:
      d = o.d;
      break;
    case Type::STRING:
      new(&s) std::string(o.s); break;
      break;
    }
  }
  virtual ~InValue(){
    using std::string;
    if(type_tag == STRING){
      this->s.~string();
    }
  }
  InValue& operator=(const InValue& o){
    if (type_tag==Type::STRING && o.type_tag==Type::STRING) {
      s = o.s;
      return *this;
    }
    if (type_tag==Type::STRING) s.~string();

    switch (o.type_tag) {
    case Type::DOUBLE: d = o.d; break;
    case Type::LLONG: i = o.i; break;
    case Type::STRING: new(&s) std::string(o.s); break;
    }
    type_tag = o.type_tag;
    return *this;
  }

  InValue(double d) : type_tag(DOUBLE), d(d) {}
  InValue(llong l) : type_tag(LLONG), i(l) {}
  InValue(std::string str) : type_tag(STRING), s(str) {}

  Type type() const{return type_tag;}
  llong llong_value() const{return i; }
  double double_value() const{return d; }
  std::string string_value() const{return s; }

  // will parse from std::string, trying in order: long integer, double, std::string
  static InValue parse(const std::string & inp);

  friend std::ostream& operator<<(std::ostream& os, const InValue& inv);

};

#endif
