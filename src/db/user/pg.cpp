#include "./pg.hpp"

// TODO: How to handle errors here, or handle this on function call
db::UserPostgres::UserPostgres(Postgres* pg) : pg(pg) {
  auto statement = this->pg->prepare(
      "SELECT id, username, password\n"
      "FROM   users\n"
      "WHERE  username=$1\n"
      "LIMIT  1;",
      1,
      std::vector<uint>(
          this->get_user_types.begin(), this->get_user_types.end()
      )
  );

  this->get_user_statement = *statement;
}

tl::expected<std::shared_ptr<entity::User>, entity::Log>
db::UserPostgres::get_user_by_username_impl(std::string& username) {

  auto result_exp = this->pg->query(
      this->get_user_statement, {username}, this->get_user_types
  );
  if (!result_exp) {
    entity::Log& log = result_exp.error();
    log.db = "users";
    return tl::unexpected<entity::Log>(log);
  }

  auto result = *result_exp;
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
