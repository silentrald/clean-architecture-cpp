#include "./redis.hpp"
#include "read.h"
#include "utils/string.hpp"
#include <cstdlib>
#include <string>

using namespace interface;

RedisStore::RedisStore(
    const std::string& host, int port, uint num_pool, Logger* logger
)
    : logger(logger), semaphore(num_pool) {

  if (logger == nullptr) {
    this->error = entity::Log{
        .msg = "Logger is not initialized",
        .file = "interfaces/store/redis.cpp",
        .function = "RedisStore::RedisStore"};
    return;
  }

  for (uint i = 0; i < num_pool; i++) {
    redisContext* ctx = redisConnect(host.c_str(), port);

    if (ctx == nullptr || ctx->err != 0) {
      for (auto& ctx : this->pool) {
        redisFree(ctx);
      }

      this->error = entity::Log{
          .msg = ctx == nullptr
                     ? "Can't allocate redis context"
                     : std::string{"Redis could not be initialized ("} +
                           ctx->errstr + ")", // NOLINT
          .file = "interfaces/store/redis.cpp",
          .function = "RedisStore::RedisStore",
      };
      redisFree(ctx);
      return;
    }

    this->pool.push_back(ctx);
  }
}

RedisStore::~RedisStore() {
  if (this->pool.empty()) {
    return;
  }

  this->logger->debug(
      "Freeing " + std::to_string(this->pool.size()) + " redis thread pools"
  );
  for (auto& ctx : this->pool) {
    redisFree(ctx);
  }
}

redisContext* RedisStore::get_context() {
  this->semaphore.acquire();

  redisContext* ctx = this->pool.front();
  this->pool.pop_front();

  return ctx;
}

void RedisStore::release_context(redisContext* ctx) {
  this->pool.push_back(ctx);

  this->semaphore.release();
}

/*** GET ***/
tl::expected<std::optional<int>, entity::Log>
RedisStore::get_int32_impl(const char* key) noexcept {
  redisContext* ctx = this->get_context();
  // clang-format off
  auto* reply = static_cast<redisReply*>(
    redisCommand(ctx, "GET %s", key)
  );
  // clang-format on
  this->release_context(ctx);

  if (reply->type == REDIS_REPLY_STRING) {
    auto val = utils::string::to_int32(reply->str);
    freeReplyObject(reply);
    if (val) {
      return val;
    }

    val.error().file = "interfaces/store/redis.cpp";
    val.error().function = "RedisStore::get_int32_impl(const char*)";
    return val;
  }

  if (reply->type == REDIS_REPLY_NIL) {
    freeReplyObject(reply);
    return std::nullopt;
  }

  entity::Log log{
      .msg = "Unknown error",
      .file = "interfaces/store/redis.cpp",
      .function = "RedisStore::get_int32_impl(const char*)"};

  if (reply->type == REDIS_REPLY_ERROR) {
    log.msg = reply->str;
  }

  freeReplyObject(reply);
  return tl::unexpected<entity::Log>(log);
}

tl::expected<std::optional<int64_t>, entity::Log>
RedisStore::get_int64_impl(const char* key) noexcept {
  redisContext* ctx = this->get_context();
  // clang-format off
  auto* reply = static_cast<redisReply*>(
    redisCommand(ctx, "GET %s", key)
  );
  // clang-format on
  this->release_context(ctx);

  if (reply->type == REDIS_REPLY_STRING) {
    auto val = utils::string::to_int64(reply->str);
    freeReplyObject(reply);
    if (val) {
      return val;
    }

    val.error().file = "interfaces/store/redis.cpp";
    val.error().function = "RedisStore::get_int64_impl(const char*)";
    return val;
  }

  if (reply->type == REDIS_REPLY_NIL) {
    freeReplyObject(reply);
    return std::nullopt;
  }

  entity::Log log{
      .msg = "Unknown error",
      .file = "interfaces/store/redis.cpp",
      .function = "RedisStore::get_int64_impl(const char*)"};

  if (reply->type == REDIS_REPLY_ERROR) {
    log.msg = reply->str;
  }

  freeReplyObject(reply);
  return tl::unexpected<entity::Log>(log);
}

tl::expected<std::optional<float>, entity::Log>
RedisStore::get_float_impl(const char* key) noexcept {
  redisContext* ctx = this->get_context();
  // clang-format off
  auto* reply = static_cast<redisReply*>(
    redisCommand(ctx, "GET %s", key)
  );
  // clang-format on
  this->release_context(ctx);

  if (reply->type == REDIS_REPLY_STRING) {
    auto val = utils::string::to_float(reply->str);
    freeReplyObject(reply);
    if (val) {
      return val;
    }

    val.error().file = "interfaces/store/redis.cpp";
    val.error().function = "RedisStore::get_float_impl(const char*)";
    return val;
  }

  if (reply->type == REDIS_REPLY_NIL) {
    freeReplyObject(reply);
    return std::nullopt;
  }

  entity::Log log{
      .msg = "Unknown error",
      .file = "interfaces/store/redis.cpp",
      .function = "RedisStore::get_float_impl(const char*)"};

  if (reply->type == REDIS_REPLY_ERROR) {
    log.msg = reply->str;
  }

  freeReplyObject(reply);
  return tl::unexpected<entity::Log>(log);
}

tl::expected<std::optional<double>, entity::Log>
RedisStore::get_double_impl(const char* key) noexcept {
  redisContext* ctx = this->get_context();
  // clang-format off
  auto* reply = static_cast<redisReply*>(
    redisCommand(ctx, "GET %s", key)
  );
  // clang-format on
  this->release_context(ctx);

  if (reply->type == REDIS_REPLY_STRING) {
    auto val = utils::string::to_double(reply->str);
    freeReplyObject(reply);
    if (val) {
      return val;
    }

    val.error().file = "interfaces/store/redis.cpp";
    val.error().function = "RedisStore::get_double_impl(const char*)";
    return val;
  }

  if (reply->type == REDIS_REPLY_NIL) {
    freeReplyObject(reply);
    return std::nullopt;
  }

  entity::Log log{
      .msg = "Unknown error",
      .file = "interfaces/store/redis.cpp",
      .function = "RedisStore::get_double_impl(const char*)"};

  if (reply->type == REDIS_REPLY_ERROR) {
    log.msg = reply->str;
  }

  freeReplyObject(reply);
  return tl::unexpected<entity::Log>(log);
}

tl::expected<std::optional<bool>, entity::Log>
RedisStore::get_bool_impl(const char* key) noexcept {
  redisContext* ctx = this->get_context();
  // clang-format off
  auto* reply = static_cast<redisReply*>(
    redisCommand(ctx, "GET %s", key)
  );
  // clang-format on
  this->release_context(ctx);

  if (reply->type == REDIS_REPLY_STRING) {
    bool val = reply->str[0] != '0';
    freeReplyObject(reply);
    return val;
  }

  if (reply->type == REDIS_REPLY_NIL) {
    freeReplyObject(reply);
    return std::nullopt;
  }

  entity::Log log{
      .msg = "Unknown error",
      .file = "interfaces/store/redis.cpp",
      .function = "RedisStore::get_double_impl(const char*)"};

  if (reply->type == REDIS_REPLY_ERROR) {
    log.msg = reply->str;
  }

  freeReplyObject(reply);
  return tl::unexpected<entity::Log>(log);
}

tl::expected<std::optional<std::string>, entity::Log>
RedisStore::get_string_impl(const char* key) noexcept {
  redisContext* ctx = this->get_context();
  // clang-format off
  auto* reply = static_cast<redisReply*>(
    redisCommand(ctx, "GET %s", key)
  );
  // clang-format on
  this->release_context(ctx);

  if (reply->type == REDIS_REPLY_STRING) {
    std::string val = reply->str;
    freeReplyObject(reply);
    return val;
  }

  if (reply->type == REDIS_REPLY_NIL) {
    freeReplyObject(reply);
    return std::nullopt;
  }

  entity::Log log{
      .msg = "Unknown error",
      .file = "interfaces/store/redis.cpp",
      .function = "RedisStore::get_double_impl(const char*)"};

  if (reply->type == REDIS_REPLY_ERROR) {
    log.msg = reply->str;
  }

  freeReplyObject(reply);
  return tl::unexpected<entity::Log>(log);
}

// GET

/*** SET ***/
bool RedisStore::set_int_impl(const char* key, int val) noexcept {
  redisContext* ctx = this->get_context();
  // clang-format off
  auto* reply = static_cast<redisReply*>(
    redisCommand(ctx, "SET %s %d", key, val)
  );
  // clang-format on
  this->release_context(ctx);

  bool set = reply->type != REDIS_REPLY_ERROR;
  freeReplyObject(reply);

  return set;
}

bool RedisStore::set_int64_impl(const char* key, int64_t val) noexcept {
  redisContext* ctx = this->get_context();
  auto* reply =
      static_cast<redisReply*>(redisCommand(ctx, "SET %s %lld", key, val));
  this->release_context(ctx);

  bool set = reply->type != REDIS_REPLY_ERROR;
  freeReplyObject(reply);

  return set;
}

bool RedisStore::set_float_impl(const char* key, float val) noexcept {
  redisContext* ctx = this->get_context();
  // clang-format off
  auto* reply = static_cast<redisReply*>(
    redisCommand(ctx, "SET %s %.7f", key, val)
  );
  // clang-format on
  this->release_context(ctx);

  bool set = reply->type != REDIS_REPLY_ERROR;
  freeReplyObject(reply);

  return set;
}

bool RedisStore::set_double_impl(const char* key, double val) noexcept {
  redisContext* ctx = this->get_context();
  // clang-format off
  auto* reply = static_cast<redisReply*>(
    redisCommand(ctx, "SET %s %.15f", key, val)
  );
  // clang-format on
  this->release_context(ctx);

  /* this->logger->debug(std::to_string(reply->type)); */

  bool set = reply->type != REDIS_REPLY_ERROR;
  freeReplyObject(reply);

  return set;
}

bool RedisStore::set_bool_impl(const char* key, bool val) noexcept {
  redisContext* ctx = this->get_context();
  // clang-format off
  auto* reply = static_cast<redisReply*>(
    redisCommand(ctx, "SET %s %d", key, static_cast<int>(val))
  );
  // clang-format on
  this->release_context(ctx);

  bool set = reply->type != REDIS_REPLY_ERROR;
  freeReplyObject(reply);

  return set;
}

bool RedisStore::set_string_impl(const char* key, const char* val) noexcept {
  redisContext* ctx = this->get_context();
  // clang-format off
  auto* reply = static_cast<redisReply*>(
    redisCommand(ctx, "SET %s %s", key, val)
  );
  // clang-format on
  this->release_context(ctx);

  bool set = reply->type != REDIS_REPLY_ERROR;
  freeReplyObject(reply);

  return set;
}

/* template <> bool RedisStore::set_impl(const char* key, const std::string&
 * val) { */
/*   redisContext* ctx = this->get_context(); */
/*   // clang-format off */
/*   auto* reply = static_cast<redisReply*>( */
/*       redisCommand(ctx, "SET %s %s", key, val.c_str()) */
/*   ); */
/*   // clang-format on */
/*   this->release_context(ctx); */

/*   bool set = reply->type != REDIS_REPLY_ERROR; */
/*   freeReplyObject(reply); */

/*   return set; */
/* } */

// SET

// TODO: Expected
bool RedisStore::exists_impl(const char* key) noexcept {
  redisContext* ctx = this->get_context();
  auto* reply = static_cast<redisReply*>(redisCommand(ctx, "EXISTS %s", key));
  this->release_context(ctx);

  if (reply->type == REDIS_REPLY_INTEGER) {
    bool val = reply->integer > 0;
    freeReplyObject(reply);
    return val;
  }

  return false;
}

bool RedisStore::del_impl(const char* key) noexcept {
  redisContext* ctx = this->get_context();
  // clang-format off
  auto* reply = static_cast<redisReply*>(
    redisCommand(ctx, "DEL %s", key)
  );
  // clang-format on
  this->release_context(ctx);

  if (reply->type == REDIS_REPLY_INTEGER) {
    bool val = reply->integer > 0;
    freeReplyObject(reply);
    return val;
  }

  return false;
}

[[nodiscard]] std::optional<entity::Log>
RedisStore::get_error_impl() const noexcept {
  return this->error;
}

