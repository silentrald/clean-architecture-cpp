#include "./singleton.hpp"

// NOLINTNEXTLINE
std::unique_ptr<interface::Store> store = nullptr;

[[nodiscard]] std::optional<entity::Log>
interface::init_store(const StoreConfig& config) {
  store = std::make_unique<interface::RedisStore>(
      config.host, config.port, config.pool
  );

  auto err = store->get_error();
  if (err != std::nullopt) {
    return err;
  }

  return std::nullopt;
}

[[nodiscard]] interface::Store* interface::get_store() {
  if (store == nullptr) {
    return nullptr;
  }

  return store.get();
}

void interface::clear_store() {
  if (store == nullptr) {
    return;
  }

  store.reset();
}

