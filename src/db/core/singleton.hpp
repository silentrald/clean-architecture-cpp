#ifndef DB_CORE_SINGLETON_HPP
#define DB_CORE_SINGLETON_HPP

#include "./pg.hpp"
#include "entities/log/main.hpp"
#include "utils/env.hpp"
#include <exception>
#include <memory>
#include <optional>

namespace db {

struct PostgresConfig {
  std::string host;
  std::string port;
  std::string user;
  std::string password;
  std::string db;
  uint pool;
};

/**
 * Initialize the postgres instance
 *
 * @param config
 * @returns if not null, returns a detailed error of
 *  why the database cannot be created
 */
[[nodiscard]] std::optional<entity::Log> init_postgres(
    const PostgresConfig& config =
        {.host = "127.0.0.1",
         .port = "5432",
         .user = "postgres",
         .password = "password",
         .db = "postgres",
         .pool = 1U}
) noexcept;

[[nodiscard]] Postgres* get_postgres() noexcept;

// For debugging purposes
void clear_postgres() noexcept;

} // namespace db

#endif

