#ifndef USER_DB_BUILD_HPP
#define USER_DB_BUILD_HPP

#include "entities/log/main.hpp"
#include "entities/user/factory.hpp"
#include "tl/expected.hpp"
#include <memory>

namespace db {
template <typename T> class IUser {
private:
  IUser() = default;

public:
  tl::expected<std::shared_ptr<entity::User>, entity::Log>
  get_user_by_username(std::string& username) noexcept {
    return static_cast<T*>(this)->get_user_by_username_impl(username);
  }

  tl::expected<std::string, entity::Log> add_user(entity::User* user) noexcept {
    return static_cast<T*>(this)->add_user_impl(user);
  }

  friend T;

  tl::expected<std::shared_ptr<entity::User>, entity::Log>
  get_user_by_username_impl(std::string& username) noexcept = delete;
  tl::expected<std::string, entity::Log> add_user_impl(entity::User* user
  ) noexcept = delete;
};
} // namespace db

#endif
