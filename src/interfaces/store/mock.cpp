#include "./mock.hpp"
#include <optional>

// TODO: Add mock implementation if needed, configure the structure passed

namespace interface {

std::optional<int> get_int_impl(const char* key) {
  return std::nullopt;
}

std::optional<int64_t> get_int64_impl(const char* key) {
  return std::nullopt;
}

std::optional<float> get_float_impl(const char* key) {
  return std::nullopt;
}

std::optional<double> get_double_impl(const char* key) {
  return std::nullopt;
}

std::optional<bool> get_bool_impl(const char* key) {
  return std::nullopt;
}

std::optional<const char*> get_string_impl(const char* key) {
  return std::nullopt;
}

bool set_int_impl(const char* key, int val) {
  return true;
}

bool set_int64_impl(const char* key, int64_t val) {
  return true;
}

bool set_float_impl(const char* key, float val) {
  return true;
}

bool set_double_impl(const char* key, double val) {
  return true;
}

bool set_bool_impl(const char* key, bool val) {
  return true;
}

bool set_string_impl(const char* key, const char* val) {
  return true;
}

bool exists_impl(const char* key) {
  return true;
}

bool del_impl(const char* key) {
  return true;
}

} // namespace interface

