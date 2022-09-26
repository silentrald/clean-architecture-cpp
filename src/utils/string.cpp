#include "./string.hpp"
#include "entities/log/main.hpp"
#include "tl/expected.hpp"
#include <algorithm>
#include <cctype>
#include <cstdint>
#include <exception>
#include <iostream>
#include <string>
#include <sys/types.h>
#include <vector>

namespace utils::string {

tl::expected<int, entity::Log> to_int32(const std::string& string) noexcept {
  try {
    return std::stoi(string);
  } catch (const std::exception& err) {
    return tl::unexpected<entity::Log>({.msg = "Could not parse to int32"});
  }
}

tl::expected<int64_t, entity::Log> to_int64(const std::string& string
) noexcept {
  try {
    return std::stoll(string);
  } catch (const std::exception& err) {
    return tl::unexpected<entity::Log>({.msg = "Could not parse to int64"});
  }
}

tl::expected<float, entity::Log> to_float(const std::string& string) noexcept {
  try {
    return std::stof(string);
  } catch (const std::exception& err) {
    return tl::unexpected<entity::Log>({.msg = "Could not parse to float"});
  }
}

tl::expected<double, entity::Log> to_double(const std::string& string
) noexcept {
  try {
    return std::stod(string);
  } catch (const std::exception& err) {
    return tl::unexpected<entity::Log>({.msg = "Could not parse to double"});
  }
}

bool iequals(const std::string& s1, const std::string& s2) noexcept {
  return std::equal(
      s1.begin(), s1.end(), s2.begin(), s2.end(),
      [](char c1, char c2) { return tolower(c1) == c2; }
  );
}

void ltrim(std::string& s) noexcept {
  s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](u_char c) {
            return std::isspace(c) == 0;
          }));
}

void rtrim(std::string& s) noexcept {
  s.erase(
      std::find_if(
          s.rbegin(), s.rend(), [](u_char c) { return std::isspace(c) == 0; }
      ).base(),
      s.end()
  );
}

void trim(std::string& s) noexcept {
  ltrim(s);
  rtrim(s);
}

} // namespace utils::string
