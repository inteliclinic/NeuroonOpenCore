#ifndef __NC_EXCEPTIONS__
#define __NC_EXCEPTIONS__

#include <stdexcept>
#include <string>

class NotImplemented : public std::logic_error{
public:
  NotImplemented() : std::logic_error("Not yet implemented.") {}
  NotImplemented(std::string what) : std::logic_error(what) {}
};

#endif
