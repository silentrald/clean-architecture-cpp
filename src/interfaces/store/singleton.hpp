#ifndef ORE_INTERFACE_MAIN_HPP
#define ORE_INTERFACE_MAIN_HPP

#include "./redis.hpp"
#include "entities/log/main.hpp"
#include "utils/env.hpp"
#include <exception>
#include <memory>
#include <mutex>

namespace interface {

const int STORE_DEFAULT_PORT = 6379;

struct StoreConfig {
  std::string host;
  int port;
  uint pool;
};

using Store = IStore<RedisStore>;

[[nodiscard]] std::optional<entity::Log> init_store(
    const StoreConfig& config =
        {
            .host = "127.0.0.1",
            .port = STORE_DEFAULT_PORT,
            .pool = 1U,
        }
);

[[nodiscard]] Store* get_store();

// For debugging
void clear_store();

} // namespace interface

#endif
