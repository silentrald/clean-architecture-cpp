#ifndef STORE_INTERFACE_BUILD_HPP
#define STORE_INTERFACE_BUILD_HPP

#include "entities/log/main.hpp"
#include "tl/expected.hpp"
#include <cstdint>
#include <optional>
#include <string>
#include <vector>

namespace interface {
template <typename T> class IStore {
private:
  IStore() = default;

public:
  // ** GETTERS **
  tl::expected<std::optional<int>, entity::Log> get_int32(const std::string& key
  ) noexcept {
    return static_cast<T*>(this)->get_int32_impl(key.c_str());
  }
  tl::expected<std::optional<int>, entity::Log> get_int32(const char* key
  ) noexcept {
    return static_cast<T*>(this)->get_int32_impl(key);
  }

  tl::expected<std::optional<int64_t>, entity::Log>
  get_int64(const std::string& key) noexcept {
    return static_cast<T*>(this)->get_int64_impl(key.c_str());
  }
  tl::expected<std::optional<int64_t>, entity::Log> get_int64(const char* key) {
    return static_cast<T*>(this)->get_int64_impl(key);
  }

  tl::expected<std::optional<float>, entity::Log>
  get_float(const std::string& key) noexcept {
    return static_cast<T*>(this)->get_float_impl(key.c_str());
  }
  tl::expected<std::optional<float>, entity::Log> get_float(const char* key
  ) noexcept {
    return static_cast<T*>(this)->get_float_impl(key);
  }

  tl::expected<std::optional<double>, entity::Log>
  get_double(const std::string& key) noexcept {
    return static_cast<T*>(this)->get_double_impl(key.c_str());
  }
  tl::expected<std::optional<double>, entity::Log> get_double(const char* key
  ) noexcept {
    return static_cast<T*>(this)->get_double_impl(key);
  }

  tl::expected<std::optional<double>, entity::Log>
  get_bool(const std::string& key) noexcept {
    return static_cast<T*>(this)->get_bool_impl(key.c_str());
  }
  tl::expected<std::optional<double>, entity::Log> get_bool(const char* key
  ) noexcept {
    return static_cast<T*>(this)->get_bool_impl(key);
  }

  tl::expected<std::optional<std::string>, entity::Log>
  get_string(const std::string& key) noexcept {
    return static_cast<T*>(this)->get_string_impl(key.c_str());
  }
  tl::expected<std::optional<std::string>, entity::Log>
  get_string(const char* key) noexcept {
    return static_cast<T*>(this)->get_string_impl(key);
  }

  tl::expected<std::optional<std::vector<uint8_t>>, entity::Log>
  get_byte_array(const std::string& key) noexcept {
    return static_cast<T*>(this)->get_byte_array_impl(key.c_str());
  }
  tl::expected<std::optional<std::vector<uint8_t>>, entity::Log>
  get_byte_array(const char* key) noexcept {
    return static_cast<T*>(this)->get_byte_array_impl(key);
  }
  // ** GETTERS **

  // ** SETTERS **
  bool set_int(const std::string& key, int val) noexcept {
    return static_cast<T*>(this)->set_int_impl(key.c_str(), val);
  }
  bool set_int(const char* key, int val) noexcept {
    return static_cast<T*>(this)->set_int_impl(key, val);
  }

  bool set_int64(const std::string& key, int64_t val) noexcept {
    return static_cast<T*>(this)->set_int64_impl(key.c_str(), val);
  }
  bool set_int64(const char* key, int64_t val) noexcept {
    return static_cast<T*>(this)->set_int64_impl(key, val);
  }

  bool set_float(const std::string& key, float val) noexcept {
    return static_cast<T*>(this)->set_float_impl(key.c_str(), val);
  }
  bool set_float(const char* key, float val) noexcept {
    return static_cast<T*>(this)->set_float_impl(key, val);
  }

  bool set_double(const std::string& key, double val) noexcept {
    return static_cast<T*>(this)->set_double_impl(key.c_str(), val);
  }
  bool set_double(const char* key, double val) noexcept {
    return static_cast<T*>(this)->set_double_impl(key, val);
  }

  bool set_bool(const std::string& key, bool val) noexcept {
    return static_cast<T*>(this)->set_bool_impl(key.c_str(), val);
  }
  bool set_bool(const char* key, bool val) noexcept {
    return static_cast<T*>(this)->set_bool_impl(key, val);
  }

  bool set_string(const std::string& key, const std::string& val) noexcept {
    return static_cast<T*>(this)->set_string_impl(key.c_str(), val.c_str());
  }
  bool set_string(const char* key, const std::string& val) noexcept {
    return static_cast<T*>(this)->set_string_impl(key, val.c_str());
  }
  bool set_string(const std::string& key, const char* val) noexcept {
    return static_cast<T*>(this)->set_string_impl(key.c_str(), val);
  }
  bool set_string(const char* key, const char* val) noexcept {
    return static_cast<T*>(this)->set_string_impl(key, val);
  }
  bool set_byte_array(
      const std::string& key, const uint8_t* val, size_t size
  ) noexcept {
    return static_cast<T*>(this)->set_byte_array_impl(key.c_str(), val, size);
  }
  bool
  set_byte_array(const char* key, const uint8_t* val, size_t size) noexcept {
    return static_cast<T*>(this)->set_byte_array_impl(key, val, size);
  }
  // ** SETTERS **

  bool exists(const std::string& key) noexcept {
    return static_cast<T*>(this)->exists_impl(key.c_str());
  }

  bool exists(const char* key) noexcept {
    return static_cast<T*>(this)->exists_impl(key);
  }

  bool del(const std::string& key) noexcept {
    return static_cast<T*>(this)->del_impl(key.c_str());
  }

  bool del(const char* key) noexcept {
    return static_cast<T*>(this)->del_impl(key);
  }

  [[nodiscard]] std::optional<entity::Log> get_error() noexcept {
    return static_cast<T*>(this)->get_error_impl();
  }
  [[nodiscard]] std::optional<entity::Log>
  get_error_impl() const noexcept = delete;

  friend T;

  tl::expected<std::optional<int>, entity::Log> get_int32_impl(const char* key
  ) noexcept = delete;
  tl::expected<std::optional<int64_t>, entity::Log>
  get_int64_impl(const char* key) noexcept = delete;
  tl::expected<std::optional<float>, entity::Log> get_float_impl(const char* key
  ) noexcept = delete;
  tl::expected<std::optional<double>, entity::Log>
  get_double_impl(const char* key) noexcept = delete;
  tl::expected<std::optional<bool>, entity::Log> get_bool_impl(const char* key
  ) noexcept = delete;
  tl::expected<std::optional<const char*>, entity::Log>
  get_string_impl(const char* key) noexcept = delete;
  tl::expected<std::optional<std::vector<uint8_t>>, entity::Log>
  get_byte_array_impl(const char* key) noexcept = delete;

  bool set_int_impl(const char* key, int val) noexcept = delete;
  bool set_int64_impl(const char* key, int64_t val) noexcept = delete;
  bool set_float_impl(const char* key, float val) noexcept = delete;
  bool set_double_impl(const char* key, double val) noexcept = delete;
  bool set_bool_impl(const char* key, bool val) noexcept = delete;
  bool set_string_impl(const char* key, const char* val) noexcept = delete;
  bool set_byte_array_impl(
      const char* key, const uint8_t* val, size_t size
  ) noexcept = delete;

  bool exists_impl(const char* key) noexcept = delete;
  bool del_impl(const char* key) noexcept = delete;
};
} // namespace interface

#endif
