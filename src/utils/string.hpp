#ifndef STRING_UTILS_HPP
#define STRING_UTILS_HPP

#include "entities/log/main.hpp"
#include <tl/expected.hpp>

namespace utils::string {

tl::expected<int, entity::Log> to_int32(const std::string& string) noexcept;
tl::expected<int64_t, entity::Log> to_int64(const std::string& string) noexcept;
tl::expected<float, entity::Log> to_float(const std::string& string) noexcept;
tl::expected<double, entity::Log> to_double(const std::string& string) noexcept;

} // namespace utils::string

#endif

