#ifndef STORE_INTERFACE_SINGLETON_HPP
#define STORE_INTERFACE_SINGLETON_HPP

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

using DefStore = RedisStore;

extern std::unique_ptr<interface::RedisStore> store;

[[nodiscard]] std::optional<entity::Log> init_store(
    const StoreConfig& config =
        {
            .host = "127.0.0.1",
            .port = STORE_DEFAULT_PORT,
            .pool = 1U,
        }
) noexcept;

template <typename Store = DefStore>
[[nodiscard]] IStore<Store>* get_store() noexcept {
  if (store == nullptr) {
    return nullptr;
  }

  return store.get();
}

// TODO: Debugging
void clear_store() noexcept;
} // namespace interface

#endif
