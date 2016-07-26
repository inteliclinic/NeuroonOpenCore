#ifndef __INVALUE__
#define __INVALUE__
#include <iostream>
#include <string>

using namespace std;

class InValue{
public:
  enum Type{DOUBLE, INT, STRING};

private:
  Type type_tag;

  union{
    double d;
    int i;
    string s;
  };
public:

  InValue(){
    new (&s) std::string();
  }

  InValue(const InValue& o){
    type_tag = Type(o.type_tag);
    switch(type_tag){
    case Type::INT:
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
  ~InValue(){
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
    case Type::INT: i = o.i; break;
    case Type::STRING: new(&s) std::string(o.s); break;
    }
    type_tag = o.type_tag;
    return *this;
  }

  InValue(double d) : type_tag(DOUBLE), d(d) {}
  InValue(int l) : type_tag(INT), i(l) {}
  InValue(string str) : type_tag(STRING), s(str) {}

  Type type() const{return type_tag;}
  int int_value() const{return i; }
  double double_value() const{return d; }
  string string_value() const{return s; }

  // will parse from string, trying in order: integer, double, string
  static InValue parse(const string & inp);

  friend ostream& operator<<(ostream& os, const InValue& inv);

};

#endif
