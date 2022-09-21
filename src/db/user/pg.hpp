#ifndef USER_DB_POSTGRES_HPP
#define USER_DB_POSTGRES_HPP

#include "../core/pg.hpp"
#include "./build.hpp"
#include "entities/log/main.hpp"
#include "tl/expected.hpp"
#include <memory>
#include <string>

namespace db {
class UserPostgres : public IUser<UserPostgres> {
private:
  std::vector<int> get_user_types = {0};
  PostgresStatement get_user_statement;
  Postgres* pg;

public:
  explicit UserPostgres(Postgres* pg);

  tl::expected<std::shared_ptr<entity::User>, entity::Log>
  get_user_by_username_impl(std::string& username) noexcept;
};
} // namespace db

#endif
