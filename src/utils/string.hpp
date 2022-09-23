#ifndef STRING_UTILS_HPP
#define STRING_UTILS_HPP

#include "entities/log/main.hpp"
#include <cstdint>
#include <tl/expected.hpp>

namespace utils::string {

tl::expected<int, entity::Log> to_int32(const std::string& string) noexcept;
tl::expected<int64_t, entity::Log> to_int64(const std::string& string) noexcept;
tl::expected<float, entity::Log> to_float(const std::string& string) noexcept;
tl::expected<double, entity::Log> to_double(const std::string& string) noexcept;
/* tl::expected<std::string, entity::Log> to_hex_string(uint8_t* byte_array, size_t size) noexcept; */
/* tl::expected<std::vector<uint8_t>, entity::Log> to_bytearray() noexcept; */

} // namespace utils::string

#endif

