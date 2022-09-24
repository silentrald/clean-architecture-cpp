#include "./string.hpp"
#include "entities/log/main.hpp"
#include "tl/expected.hpp"
#include <cstdint>
#include <exception>
#include <iostream>
#include <string>
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

} // namespace utils::string
