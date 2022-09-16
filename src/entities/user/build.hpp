#ifndef USER_ENTITY_BUILD_HPP
#define USER_ENTITY_BUILD_HPP

#include "entities/log/main.hpp"
#include "tl/expected.hpp"
#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <vector>

namespace entity {
template <class IUserCrypto> class IUser {
private:
  std::string id{};
  std::string username{};
  std::string password{};
  std::string hashed{};

  const int PASSWORD_MIN = 8;
  const int PASSWORD_MAX = 60;

public:
  IUser(std::string& id, std::string& username, std::string& hashed)
      : id(id), username(username), hashed(hashed) {}

  IUser(std::string& id, std::string& username) : id(id), username(username) {}

  explicit IUser(std::string& username) : username(username) {}

  std::optional<Log> set_password(std::string& password) noexcept;

  // GETTERS
  [[nodiscard]] const std::string& get_id() const;
  [[nodiscard]] const std::string& get_username() const;
  [[nodiscard]] const std::string& get_hashed() const;

  [[nodiscard]] std::optional<Log> hash_password() noexcept;
  [[nodiscard]] tl::expected<bool, Log> verify_password(std::string& password
  ) noexcept;
};

const char* const USER_ENTITY_FILE = "entities/user/build.hpp";

// Implementation
template <class IUserCrypto>
std::optional<entity::Log>
IUser<IUserCrypto>::set_password(std::string& password) noexcept {

  uint len = password.size();
  if (len < this->PASSWORD_MIN) {
    return Log{
        .msg = "Validation Error",
        .file = USER_ENTITY_FILE,
        .errors = {"password: min"}};
  }

  if (len > this->PASSWORD_MAX) {
    return Log{
        .msg = "Validation Error",
        .file = USER_ENTITY_FILE,
        .errors = {"password: max"}};
  }

  // TODO: Add more password validation here

  this->password = std::move(password);
  return std::nullopt;
}

template <class IUserCrypto>
[[nodiscard]] const std::string& IUser<IUserCrypto>::get_id() const {
  return this->id;
}

template <class IUserCrypto>
[[nodiscard]] const std::string& IUser<IUserCrypto>::get_username() const {
  return this->username;
}

template <class IUserCrypto>
[[nodiscard]] const std::string& IUser<IUserCrypto>::get_hashed() const {
  return this->hashed;
}

template <class IUserCrypto>
std::optional<Log> IUser<IUserCrypto>::hash_password() noexcept {
  tl::expected<std::string, entity::Log> hash = IUserCrypto::hash(this->password);
  if (!hash) {
    return hash.error();
  }

  this->hashed = hash;
  return std::nullopt;
}

template <class IUserCrypto>
tl::expected<bool, Log>
IUser<IUserCrypto>::verify_password(std::string& password) noexcept {
  return IUserCrypto::verify(password, this->hashed);
}
} // namespace entity

#endif
