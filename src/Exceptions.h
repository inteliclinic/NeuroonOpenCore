#include <stdexcept>

class NotImplemented : public std::logic_error{
public:
  NotImplemented() : std::logic_error("Not yet implemented.") {}
  NotImplemented(std::string what) : std::logic_error(what) {}
};
