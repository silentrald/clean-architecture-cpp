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

/* const char* const HEX = "0123456789ABCDEF"; */

/* tl::expected<std::string, entity::Log> */
/* to_hex_string(uint8_t* byte_array, size_t size) noexcept { */
/*   std::vector<char> hex_string; */
/*   try { */
/*     hex_string.resize(size * 2); */
/*   } catch (std::exception& err) { */
/*     tl::unexpected<entity::Log>({.msg = err.what()}); */
/*   } */

/*   uint8_t tmp = 0; */
/*   for (int i = 0; i < size; i++) { */
/*     tmp = byte_array[i]; */
/*     hex_string[i * 2] = HEX[tmp & 0xf0 >> 0x4];     // Upper bit */
/*     hex_string[i * 2 + 1] = HEX[tmp & 0x0f]; // Lower bit */
/*   } */

/*   std::string string = hex_string.data(); */
/*   std::cout << size << "\n" << string << "\n"; */

/*   return hex_string.data(); */
/* } */

} // namespace utils::string
