#ifndef STORE_INTERFACE_MOCK_HPP
#define STORE_INTERFACE_MOCK_HPP

#include "./build.hpp"
#include <cstddef>
#include <optional>

namespace interface {
struct MockStoreConfig {};

class MockStore : public IStore<MockStore> {
private:
  MockStoreConfig config;

public:
  explicit MockStore(const MockStoreConfig& config) : config(config) {}

  std::optional<int> get_int_impl(const char* key) noexcept;
  std::optional<int64_t> get_int64_impl(const char* key) noexcept;
  std::optional<float> get_float_impl(const char* key) noexcept;
  std::optional<double> get_double_impl(const char* key) noexcept;
  std::optional<bool> get_bool_impl(const char* key) noexcept;
  std::optional<const char*> get_string_impl(const char* key) noexcept;

  bool set_int_impl(const char* key, int val) noexcept;
  bool set_int64_impl(const char* key, int64_t val) noexcept;
  bool set_float_impl(const char* key, float val) noexcept;
  bool set_double_impl(const char* key, double val) noexcept;
  bool set_bool_impl(const char* key, bool val) noexcept;
  bool set_string_impl(const char* key, const char* val) noexcept;
  /* bool set_string_impl(const char* key, const char* val, size_t size) noexcept; */

  bool exists_impl(const char* key) noexcept;
  bool del_impl(const char* key) noexcept;
};
} // namespace interface

#endif

