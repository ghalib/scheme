#ifndef _UTIL_H
#define _UTIL_H

#include <iostream>
#include <string>
#include <sstream>
#include <exception>



// A few utility functions plus our exception class.

#define PR(x) \
  std::cout << #x << " = " << x << std::endl;

class Exception : public std::exception {
private:
  std::string msg;
public:
  Exception(const std::string&);
  virtual const char* what() const throw(); 
  virtual ~Exception() throw() {}
};

template<typename To, typename From>
To convert(const From& s)
{
  std::stringstream ss(s);
  To t;
  ss >> t;
  return t;
}

bool is_number(const std::string&);
size_t next_index(const std::string&, size_t);


#endif
