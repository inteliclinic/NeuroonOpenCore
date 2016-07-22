#include <string>

using namespace std;

class InMsg{
public:
  enum class Type{INT, DOUBLE, STRING};
private:
  unsigned long _value_timestamp = 0;
  unsigned long _recvd_timestamp = 0;
  struct Value{
    Type type;
    union{
      int i;
      double d;
      std::string s;
    };

    Value(){
      new (&s) std::string();
    }

    Value(const Value& o){
      type = Type(o.type);
      switch(type){
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
    ~Value(){
      if(type == Type::STRING){
        s.string::~string();
      }
    }
    Value& operator=(const Value& o){
			if (type==Type::STRING && o.type==Type::STRING) {
				s = o.s;
				return *this;
			}

			if (type==Type::STRING) s.~string();

			switch (o.type) {
			case Type::DOUBLE: d = o.d; break;
			case Type::INT: i = o.i; break;
			case Type::STRING: new(&s) std::string(o.s); break;
			}
			type = o.type;
			return *this;
		}
  }_v;

public:
  Type type() const{return _v.type;}

  int int_value() const{return _v.i; }
  double double_value() const{return _v.d; }
  std::string string_value() const{return _v.s; }

  InMsg(string value, long timestamp=0) : _value_timestamp(timestamp){
    _v.type = Type::STRING;
    _v.s = value;
  }
  InMsg(double value, long timestamp=0) : _value_timestamp(timestamp){
    _v.type = Type::DOUBLE;
    _v.d = value;
  }
  InMsg(int value, long timestamp=0) : _value_timestamp(timestamp){
    _v.type = Type::INT;
    _v.i = value;
  }
  InMsg(InMsg&& o) noexcept : _v(std::move(o._v)) {}
  InMsg(const InMsg& o) : _v(o._v) {}
  InMsg& operator=(InMsg o){
    std::swap(o, *this);
    return *this;
  }

  unsigned long value_timestamp() const{
    return _value_timestamp;
  }
  void set_recv_timestamp(unsigned long t){
    _recvd_timestamp = t;
  }

};
