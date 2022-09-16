#include "./redis.hpp"

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
std::optional<int> RedisStore::get_int_impl(const char* key) {
  redisContext* ctx = this->get_context();
  // clang-format off
  auto* reply = static_cast<redisReply*>(
    redisCommand(ctx, "GET %s", key)
  );
  // clang-format on
  this->release_context(ctx);

  if (reply->type == REDIS_REPLY_INTEGER) {
    int val = static_cast<int>(reply->integer);
    freeReplyObject(reply);
    return reply->integer;
  }

  if (reply->type == REDIS_REPLY_NIL) {
    freeReplyObject(reply);
    return std::nullopt;
  }

  freeReplyObject(reply);
  return std::nullopt;
}

std::optional<int64_t> RedisStore::get_int64_impl(const char* key) {
  redisContext* ctx = this->get_context();
  // clang-format off
  auto* reply = static_cast<redisReply*>(
    redisCommand(ctx, "GET %s", key)
  );
  // clang-format on
  this->release_context(ctx);

  if (reply->type == REDIS_REPLY_INTEGER) {
    long long val = reply->integer;
    freeReplyObject(reply);
    return val;
  }

  if (reply->type == REDIS_REPLY_NIL) {
    freeReplyObject(reply);
    return std::nullopt;
  }

  freeReplyObject(reply);
  return std::nullopt;
}

std::optional<float> RedisStore::get_float_impl(const char* key) {
  return std::nullopt;
  redisContext* ctx = this->get_context();
  // clang-format off
  auto* reply = static_cast<redisReply*>(
    redisCommand(ctx, "GET %s", key)
  );
  // clang-format on
  this->release_context(ctx);

  if (reply->type == REDIS_REPLY_DOUBLE) {
    double val = std::stod(reply->str);
    freeReplyObject(reply);
    return val;
  }

  if (reply->type == REDIS_REPLY_NIL) {
    freeReplyObject(reply);
    return std::nullopt;
  }

  freeReplyObject(reply);
  return std::nullopt;
}

std::optional<double> RedisStore::get_double_impl(const char* key) {
  redisContext* ctx = this->get_context();
  // clang-format off
  auto* reply = static_cast<redisReply*>(
    redisCommand(ctx, "GET %s", key)
  );
  // clang-format on
  this->release_context(ctx);

  if (reply->type == REDIS_REPLY_DOUBLE) {
    float val = std::stof(reply->str);
    freeReplyObject(reply);
    return val;
  }

  if (reply->type == REDIS_REPLY_NIL) {
    freeReplyObject(reply);
    return std::nullopt;
  }

  freeReplyObject(reply);
  return std::nullopt;
}

std::optional<bool> RedisStore::get_bool_impl(const char* key) {
  redisContext* ctx = this->get_context();
  // clang-format off
  auto* reply = static_cast<redisReply*>(
    redisCommand(ctx, "GET %s", key)
  );
  // clang-format on
  this->release_context(ctx);

  if (reply->type == REDIS_REPLY_BOOL) {
    bool val = reply->integer > 0;
    freeReplyObject(reply);
    return val;
  }

  if (reply->type == REDIS_REPLY_NIL) {
    freeReplyObject(reply);
    return std::nullopt;
  }

  freeReplyObject(reply);
  return std::nullopt;
}

std::optional<const char*> RedisStore::get_string_impl(const char* key) {
  redisContext* ctx = this->get_context();
  // clang-format off
  auto* reply = static_cast<redisReply*>(
    redisCommand(ctx, "GET %s", key)
  );
  // clang-format on
  this->release_context(ctx);

  if (reply->type == REDIS_REPLY_STRING) {
    const char* val = reply->str;
    freeReplyObject(reply);
    return val;
  }

  if (reply->type == REDIS_REPLY_NIL) {
    freeReplyObject(reply);
    return std::nullopt;
  }

  freeReplyObject(reply);
  return std::nullopt;
}

// GET

/*** SET ***/
bool RedisStore::set_int_impl(const char* key, int val) {
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

bool RedisStore::set_int64_impl(const char* key, int64_t val) {
  redisContext* ctx = this->get_context();
  auto* reply =
      static_cast<redisReply*>(redisCommand(ctx, "SET %s %lld", key, val));
  this->release_context(ctx);

  bool set = reply->type != REDIS_REPLY_ERROR;
  freeReplyObject(reply);

  return set;
}

bool RedisStore::set_float_impl(const char* key, float val) {
  redisContext* ctx = this->get_context();
  // clang-format off
  auto* reply = static_cast<redisReply*>(
    redisCommand(ctx, "SET %s %f", key, val)
  );
  // clang-format on
  this->release_context(ctx);

  bool set = reply->type != REDIS_REPLY_ERROR;
  freeReplyObject(reply);

  return set;
}

bool RedisStore::set_double_impl(const char* key, double val) {
  redisContext* ctx = this->get_context();
  // clang-format off
  auto* reply = static_cast<redisReply*>(
    redisCommand(ctx, "SET %s %lf", key, val)
  );
  // clang-format on
  this->release_context(ctx);

  bool set = reply->type != REDIS_REPLY_ERROR;
  freeReplyObject(reply);

  return set;
}

bool RedisStore::set_bool_impl(const char* key, bool val) {
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

bool RedisStore::set_string_impl(const char* key, const char* val) {
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
bool RedisStore::exists_impl(const char* key) {
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

bool RedisStore::del_impl(const char* key) {
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

