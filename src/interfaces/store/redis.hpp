#ifndef ORE_INTERFACE_REDIS_HPP
#define ORE_INTERFACE_REDIS_HPP

#include "./build.hpp"
#include "entities/log/main.hpp"
#include "interfaces/logger/singleton.hpp"
#include <cstdint>
#include <deque>
#include <hiredis.h>
#include <memory>
#include <optional>
#include <semaphore>

namespace interface {
class RedisStore : public IStore<RedisStore> {
private:
  std::deque<redisContext*> pool;
  std::counting_semaphore<> semaphore;
  Logger* logger = nullptr;
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
      const std::string& host, int port, uint num_pool = 1U,
      Logger* logger = get_logger()
  );
  ~RedisStore();

  std::optional<int> get_int_impl(const char* key);
  std::optional<int64_t> get_int64_impl(const char* key);
  std::optional<float> get_float_impl(const char* key);
  std::optional<double> get_double_impl(const char* key);
  std::optional<bool> get_bool_impl(const char* key);
  std::optional<const char*> get_string_impl(const char* key);

  bool set_int_impl(const char* key, int val);
  bool set_int64_impl(const char* key, int64_t val);
  bool set_float_impl(const char* key, float val);
  bool set_double_impl(const char* key, double val);
  bool set_bool_impl(const char* key, bool val);
  bool set_string_impl(const char* key, const char* val);

  bool exists_impl(const char* key);
  bool del_impl(const char* key);

  // Error handling
  [[nodiscard]] std::optional<entity::Log> get_error_impl() const noexcept;
};
} // namespace interface

#endif
