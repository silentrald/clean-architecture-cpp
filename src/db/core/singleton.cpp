#include "./singleton.hpp"

// NOLINTNEXTLINE
std::unique_ptr<db::Postgres> postgres = nullptr;

[[nodiscard]] std::optional<entity::Log>
db::init_postgres(const PostgresConfig& config) noexcept {
    postgres = std::make_unique<db::Postgres>(
        config.host, config.port, config.user, config.password, config.db,
        config.pool
    );
  auto err = postgres->get_error();
  if (err != std::nullopt) {
    return err;
  }

  return std::nullopt;
}

[[nodiscard]] db::Postgres* db::get_postgres() noexcept {
  if (postgres == nullptr) {
    return nullptr;
  }

  return postgres.get();
}

void db::clear_postgres() noexcept {
  if (postgres == nullptr) {
    return;
  }

  postgres.reset();
}
