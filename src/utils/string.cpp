#include "./string.hpp"
#include <exception>
#include <string>

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

} // namespace utils::string
