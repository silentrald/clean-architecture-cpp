#include "./factory.hpp"
#include "entities/user/build.hpp"

const int USERNAME_MIN = 8;
const int USERNAME_MAX = 60;

/**
 * Validates the username of the user
 */
const char* validate_username(const std::string& username) {
  if (username.length() < USERNAME_MIN) {
    return "username: min";
  }

  if (username.length() > USERNAME_MAX) {
    return "username: max";
  }

  return nullptr;
}

tl::expected<std::shared_ptr<entity::User>, entity::Log>
entity::make_user(std::string& id, std::string& username, std::string& hashed) {
  std::vector<std::string> errors;
  const char* err = validate_username(username);
  if (err != nullptr) {
    errors.emplace_back(err);
  }

  if (!errors.empty()) {
    return tl::unexpected<entity::Log>(
        {.msg = ERROR_MSG,
         .file = "src/entities/factory.cpp",
         .function = "entity::make_user(std::string& id, std::string& "
                     "username, std::string& hashed)",
         .errors = errors}
    );
  }

  return std::make_unique<entity::User>(id, username, hashed);
}

tl::expected<std::shared_ptr<entity::User>, entity::Log>
entity::make_user(std::string& id, std::string& username) {
  std::vector<std::string> errors;
  const char* err = validate_username(username);
  if (err != nullptr) {
    errors.emplace_back(err);
  }

  if (!errors.empty()) {
    return tl::unexpected<entity::Log>(
        {.msg = ERROR_MSG,
         .file = "src/entities/factory.cpp",
         .function =
             "entity::make_user(std::string& id, std::string& username)",
         .errors = errors}
    );
  }

  return std::make_unique<entity::User>(id, username);
}

tl::expected<std::shared_ptr<entity::User>, entity::Log>
entity::make_user(std::string& username) {
  std::vector<std::string> errors;
  const char* err = validate_username(username);
  if (err != nullptr) {
    errors.emplace_back(err);
  }

  if (!errors.empty()) {
    return tl::unexpected<entity::Log>(
        {.msg = ERROR_MSG,
         .file = "src/entities/factory.cpp",
         .function = "entity::make_user(std::string& id)",
         .errors = errors}
    );
  }

  return std::make_unique<entity::User>(username);
}

