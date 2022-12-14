#include "./pg.hpp"
#include "entities/log/main.hpp"
#include "tl/expected.hpp"
#include <string>
#include <vector>

// TODO: How to handle errors here, or handle this on function call
db::UserPostgres::UserPostgres(Postgres* pg) : pg(pg) {
  auto statement = this->pg->prepare(
      "SELECT id, username, password\n"
      "FROM   users\n"
      "WHERE  username=$1\n"
      "LIMIT  1;",
      this->get_user_types.size(),
      std::vector<uint>(
          this->get_user_types.begin(), this->get_user_types.end()
      )
  );

  this->get_user_statement = *statement;

  statement = this->pg->prepare(
      "INSERT INTO users (username, password)\n"
      "VALUES ($1, $2)\n"
      "RETURNING id;",
      this->add_user_types.size(),
      std::vector<uint>(
          this->add_user_types.begin(), this->add_user_types.end()
      )
  );

  this->add_user_statement = *statement;
}

tl::expected<std::shared_ptr<entity::User>, entity::Log>
db::UserPostgres::get_user_by_username_impl(std::string& username) noexcept {
  auto exp_result = this->pg->query(
      this->get_user_statement, {username}, this->get_user_types
  );
  if (!exp_result) {
    entity::Log& log = exp_result.error();
    log.db = "users";
    return tl::unexpected<entity::Log>(log);
  }

  auto result = *exp_result;
  if (result->get_count() == 0) {
    return tl::unexpected<entity::Log>(
        {.msg = "User not found",
         .file = "db/user/pg.cpp",
         .function = "get_user_by_username_impl",
         .db = "users"}
    );
  }

  std::string id = result->get_cell<char*>(0, 0);
  std::string hashed = result->get_cell<char*>(0, 2);
  auto user = entity::make_user(id, username, hashed);
  if (!user) {
    return tl::unexpected<entity::Log>(user.error());
  }

  return *user;
}

tl::expected<std::string, entity::Log>
db::UserPostgres::add_user_impl(entity::User* user) noexcept {
  auto exp_result = this->pg->query(
    this->add_user_statement, {user->get_username(), user->get_hashed()}, this->add_user_types
  );
  if (!exp_result) {
    entity::Log& log = exp_result.error();
    log.db = "users";
    return tl::unexpected<entity::Log>(log);
  }

  auto result = *exp_result;
  return result->get_cell<char*>(0, 0); // id
}
