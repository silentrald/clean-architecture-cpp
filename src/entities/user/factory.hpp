#ifndef USER_ENTITY_FACTORY_HPP
#define USER_ENTITY_FACTORY_HPP

#include "./build.hpp"
#include "entities/log/main.hpp"
#include "interfaces/crypto/singleton.hpp"
#include <memory>
#include <tl/expected.hpp>

namespace entity {
struct UserCrypto {
  /**
   * Hashes the argument passed
   *
   * @param text - plain text to be hashed
   * @returns the hashed string
   */
  static tl::expected<std::string, entity::Log> hash(std::string& text) noexcept {
    return interface::get_crypto()->hash(text);
  }

  /**
   * Verifies if the password is the hashed string
   *
   * @param text
   * @param hashed
   * @returns true if the password matches the hash else false
   */
  static tl::expected<bool, entity::Log>
  verify(std::string& text, std::string& hashed) noexcept {
    return interface::get_crypto()->verify(text, hashed);
  }
};

using User = IUser<UserCrypto>;

/**
 * Creates a user with id, username and hashed
 *
 * @param id
 * @param username
 * @param hashed
 * @returns
 *   Returns a user if inputs are valid
 *   else returns a log object
 */
tl::expected<std::shared_ptr<User>, Log>
make_user(std::string& id, std::string& username, std::string& hashed);

/**
 * Creates a user with id and username
 *
 * @param id
 * @param username
 * @returns
 *   Returns a user if inputs are valid
 *   else returns a log object
 */
tl::expected<std::shared_ptr<User>, Log>
make_user(std::string& id, std::string& username);

/**
 * Creates a user with id
 *
 * @param id
 * @returns
 *   Returns a user if inputs are valid
 *   else returns a log object
 */
tl::expected<std::shared_ptr<User>, Log> make_user(std::string& username);

} // namespace entity

#endif

