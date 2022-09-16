#ifndef STORE_INTERFACE_BUILD_HPP
#define STORE_INTERFACE_BUILD_HPP

#include "entities/log/main.hpp"
#include <cstdint>
#include <optional>
#include <string>

namespace interface {
template <typename T> class IStore {
private:
  IStore() = default;

public:
  // ** GETTERS **
  std::optional<int> get_int(const std::string& key) {
    return static_cast<T*>(this)->get_int_impl(key.c_str());
  }
  std::optional<int> get_int(const char* key) {
    return static_cast<T*>(this)->get_int_impl(key);
  }

  std::optional<int64_t> get_int64(const std::string& key) {
    return static_cast<T*>(this)->get_int64_impl(key.c_str());
  }
  std::optional<int64_t> get_int64(const char* key) {
    return static_cast<T*>(this)->get_int64_impl(key);
  }

  std::optional<float> get_float(const std::string& key) {
    return static_cast<T*>(this)->get_float_impl(key.c_str());
  }
  std::optional<float> get_float(const char* key) {
    return static_cast<T*>(this)->get_float_impl(key);
  }

  std::optional<double> get_double(const std::string& key) {
    return static_cast<T*>(this)->get_double_impl(key.c_str());
  }
  std::optional<double> get_double(const char* key) {
    return static_cast<T*>(this)->get_double_impl(key);
  }

  std::optional<double> get_bool(const std::string& key) {
    return static_cast<T*>(this)->get_bool_impl(key.c_str());
  }
  std::optional<double> get_bool(const char* key) {
    return static_cast<T*>(this)->get_bool_impl(key);
  }

  std::optional<const char*> get_char_str(const std::string& key) {
    return static_cast<T*>(this)->get_string_impl(key.c_str());
  }
  std::optional<const char*> get_char_str(const char* key) {
    return static_cast<T*>(this)->get_string_impl(key);
  }
  std::optional<std::string> get_string(const std::string& key) {
    return static_cast<T*>(this)->get_string_impl(key.c_str());
  }
  std::optional<std::string> get_string(const char* key) {
    return static_cast<T*>(this)->get_string_impl(key);
  }
  // ** GETTERS **

  // ** SETTERS **
  bool set_int(const std::string& key, int val) {
    return static_cast<T*>(this)->set_int_impl(key.c_str(), val);
  }
  bool set_int(const char* key, int val) {
    return static_cast<T*>(this)->set_int_impl(key, val);
  }

  bool set_int64(const std::string& key, int64_t val) {
    return static_cast<T*>(this)->set_int64_impl(key.c_str(), val);
  }
  bool set_int64(const char* key, int64_t val) {
    return static_cast<T*>(this)->set_int64_impl(key, val);
  }

  bool set_float(const std::string& key, float val) {
    return static_cast<T*>(this)->set_float_impl(key.c_str(), val);
  }
  bool set_float(const char* key, float val) {
    return static_cast<T*>(this)->set_float_impl(key, val);
  }

  bool set_double(const std::string& key, double val) {
    return static_cast<T*>(this)->set_double_impl(key.c_str(), val);
  }
  bool set_double(const char* key, double val) {
    return static_cast<T*>(this)->set_double_impl(key, val);
  }

  bool set_bool(const std::string& key, bool val) {
    return static_cast<T*>(this)->set_bool_impl(key.c_str(), val);
  }
  bool set_bool(const char* key, bool val) {
    return static_cast<T*>(this)->set_bool_impl(key, val);
  }

  bool set_string(const std::string& key, const std::string& val) {
    return static_cast<T*>(this)->set_string_impl(key.c_str(), val.c_str());
  }
  bool set_string(const char* key, const std::string& val) {
    return static_cast<T*>(this)->set_string_impl(key, val.c_str());
  }
  bool set_string(const std::string& key, const char* val) {
    return static_cast<T*>(this)->set_string_impl(key.c_str(), val);
  }
  bool set_string(const char* key, const char* val) {
    return static_cast<T*>(this)->set_string_impl(key, val);
  }
  // ** SETTERS **

  bool exists(const std::string& key) {
    return static_cast<T*>(this)->exists_impl(key.c_str());
  }

  bool exists(const char* key) {
    return static_cast<T*>(this)->exists_impl(key);
  }

  bool del(const std::string& key) {
    return static_cast<T*>(this)->del_impl(key.c_str());
  }

  bool del(const char* key) {
    return static_cast<T*>(this)->del_impl(key);
  }

  [[nodiscard]] std::optional<entity::Log> get_error() {
    return static_cast<T*>(this)->get_error_impl();
  }
  [[nodiscard]] std::optional<entity::Log> get_error_impl() const noexcept = delete;

  friend T;

  std::optional<int> get_int_impl(const char* key) = delete;
  std::optional<int64_t> get_int64_impl(const char* key) = delete;
  std::optional<float> get_float_impl(const char* key) = delete;
  std::optional<double> get_double_impl(const char* key) = delete;
  std::optional<bool> get_bool_impl(const char* key) = delete;
  std::optional<const char*> get_string_impl(const char* key) = delete;

  bool set_int_impl(const char* key, int val) = delete;
  bool set_int64_impl(const char* key, int64_t val) = delete;
  bool set_float_impl(const char* key, float val) = delete;
  bool set_double_impl(const char* key, double val) = delete;
  bool set_bool_impl(const char* key, bool val) = delete;
  bool set_string_impl(const char* key, const char* val) = delete;

  bool exists_impl(const char* key) = delete;
  bool del_impl(const char* key) = delete;
};
} // namespace interface

#endif
