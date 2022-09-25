#ifndef USER_DB_POSTGRES_HPP
#define USER_DB_POSTGRES_HPP

#include "../core/pg.hpp"
#include "./build.hpp"
#include "entities/log/main.hpp"
#include "tl/expected.hpp"
#include <memory>
#include <string>
#include <vector>

namespace db {
class UserPostgres : public IUser<UserPostgres> {
private:
  std::vector<int> get_user_types = {0};
  PostgresStatement get_user_statement;
  std::vector<int> add_user_types = {0, 0};
  PostgresStatement add_user_statement;
  Postgres* pg;

public:
  explicit UserPostgres(Postgres* pg);

  tl::expected<std::shared_ptr<entity::User>, entity::Log>
  get_user_by_username_impl(std::string& username) noexcept;
  tl::expected<std::string, entity::Log> add_user_impl(entity::User* user
  ) noexcept;
};
} // namespace db

#endif
