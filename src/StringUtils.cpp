#include "StringUtils.h"

std::string trim_right(const std::string& s,
                                   const std::string& delimiters) {
  return s.substr( 0, s.find_last_not_of( delimiters ) + 1 );
}

std::string trim_left(const std::string& s,
                                  const std::string& delimiters) {
  return s.substr( s.find_first_not_of( delimiters ) );
}

std::string trim(const std::string& s,
                             const std::string& delimiters) {
  return trim_left( trim_right( s, delimiters ), delimiters );
}
