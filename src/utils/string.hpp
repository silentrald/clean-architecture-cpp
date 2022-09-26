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

/**
 * Case insensitive comparison between 2 strings
 *
 * @params s1 - To compare with
 * @params s2 - All characters should be small caps
 * @returns bool
 */
bool iequals(const std::string& s1, const std::string& s2) noexcept;

/**
 * Removes whitespace characters on the left side of the string
 * 
 * @params s - string to be trimmed
 */
void ltrim(std::string& s) noexcept;

/**
 * Removes whitespace characters on the right side of the string
 *
 * @params s - string to be trimmed
 */
void rtrim(std::string& s) noexcept;

/**
 * Removes whitespace characters on both side of the string
 *
 * @params s - string to be trimmed
 */
void trim(std::string& s) noexcept;

} // namespace utils::string

#endif

