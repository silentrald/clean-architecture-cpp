#ifndef STORE_INTERFACE_REDIS_HPP
#define STORE_INTERFACE_REDIS_HPP

#include "./build.hpp"
#include "entities/log/main.hpp"
#include "interfaces/logger/singleton.hpp"
#include "tl/expected.hpp"
#include <cstdint>
#include <deque>
#include <hiredis.h>
#include <memory>
#include <optional>
#include <semaphore>
#include <string>

namespace interface {
class RedisStore : public IStore<RedisStore> {
private:
  std::deque<redisContext*> pool;
  std::counting_semaphore<> semaphore;
  /* Logger* logger = nullptr; */
  std::optional<entity::Log> error = std::nullopt;

  redisContext* get_context();
  void release_context(redisContext* ctx);

public:
  RedisStore() = delete;
  RedisStore(const RedisStore&) = delete;
  RedisStore(RedisStore&&) = delete;
  RedisStore& operator=(const RedisStore&) = delete;
  RedisStore& operator=(RedisStore&&) = delete;

  RedisStore(
      const std::string& host, int port, uint num_pool = 1U
      /* Logger* logger = get_logger() */
  );
  ~RedisStore();

  tl::expected<std::optional<int>, entity::Log> get_int32_impl(const char* key
  ) noexcept;
  tl::expected<std::optional<int64_t>, entity::Log>
  get_int64_impl(const char* key) noexcept;
  tl::expected<std::optional<float>, entity::Log> get_float_impl(const char* key
  ) noexcept;
  tl::expected<std::optional<double>, entity::Log>
  get_double_impl(const char* key) noexcept;
  tl::expected<std::optional<bool>, entity::Log> get_bool_impl(const char* key
  ) noexcept;
  tl::expected<std::optional<std::string>, entity::Log>
  get_string_impl(const char* key) noexcept;

  bool set_int_impl(const char* key, int val) noexcept;
  bool set_int64_impl(const char* key, int64_t val) noexcept;
  bool set_float_impl(const char* key, float val) noexcept;
  bool set_double_impl(const char* key, double val) noexcept;
  bool set_bool_impl(const char* key, bool val) noexcept;
  /* bool set_string_impl(const char* key, const std::string& val) noexcept; */
  bool set_string_impl(const char* key, const char* val) noexcept;

  bool exists_impl(const char* key) noexcept;
  bool del_impl(const char* key) noexcept;

  // Error handling
  [[nodiscard]] std::optional<entity::Log> get_error_impl() const noexcept;
};
} // namespace interface

#endif
