#include "./singleton.hpp"

std::unique_ptr<interface::RedisStore> interface::store = nullptr;

[[nodiscard]] std::optional<entity::Log>
interface::init_store(const StoreConfig& config) noexcept {
  store = std::make_unique<interface::RedisStore>(
      config.host, config.port, config.pool
  );

  auto err = store->get_error();
  if (err != std::nullopt) {
    return err;
  }

  return std::nullopt;
}

void interface::clear_store() noexcept {
  if (store == nullptr) {
    return;
  }

  store.reset();
}


