#include "util.h"

Exception::Exception(const std::string& _msg) : msg(_msg) {}

const char* Exception::what() const throw()
{
  return msg.c_str();
}

bool is_number(const std::string& s)
{
  std::stringstream ss(s);
  double d;
  if (ss >> d)
    return true;
  else
    return false;
}

size_t next_index(const std::string& sexp, size_t current)
{
  return sexp.find_first_of("() ", current);
}
