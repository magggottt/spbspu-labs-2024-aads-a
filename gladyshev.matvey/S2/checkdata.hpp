#ifndef CHECKDATA_HPP
#define CHECKDATA_HPP

#include <string>

namespace gladyshev
{
  bool isNumber(const std::string& s);
  bool isOperator(const std::string& s);
  bool checkQuality(const std::string& top, const std::string& now);
}

#endif