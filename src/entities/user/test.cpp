#include "./factory.hpp"
#include <catch2/catch_all.hpp>
#include <optional>

const int USERNAME_OVER_MAX = 61;
const int PASSWORD_OVER_MAX = 61;

using Crypto = entity::UserCrypto;

// NOLINTNEXTLINE (cert-err58-cpp)
TEST_CASE("User Entity", "Entity") {
  SECTION("All fields with hashing the password") {
    std::string id = "id";
    std::string username = "username";
    std::string password = "password";

    auto expected_user = entity::make_user(id, username);

    REQUIRE(expected_user);

    auto& user = expected_user.value();
    REQUIRE(user->get_id() == id);
    REQUIRE(user->get_username() == username);

    auto error = user->set_password(password);
    REQUIRE(!error);

    error = user->hash_password();
    REQUIRE(!error);

    // Check if the hash functions work
    password = "password";
    auto verified = user->verify_password(password);
    REQUIRE(verified);
    REQUIRE(verified.value());

    // Password Mismatch
    password = "some-random-password";
    verified = user->verify_password(password);
    REQUIRE(verified);
    REQUIRE(!verified.value());
  }

  SECTION("All fields with setting the hashed password") {
    std::string id = "id";
    std::string username = "username";
    std::string password = "password";

    // Hash the password
    auto expected_hashed = Crypto::hash(password);
    REQUIRE(expected_hashed);
    std::string hashed = *expected_hashed;

    auto expected_user = entity::make_user(id, username, hashed);
    REQUIRE(expected_user);
    auto& user = expected_user.value();

    REQUIRE(user->get_id() == id);
    REQUIRE(user->get_username() == username);

    auto verified = user->verify_password(password);
    REQUIRE(verified);
    REQUIRE(verified.value());
  }

  SECTION("Success with only username") {
    std::string username = "username";
    auto expected_user = entity::make_user(username);
    REQUIRE(expected_user);

    auto& user = expected_user.value();
    REQUIRE(user->get_id().empty());
    REQUIRE(user->get_username() == "username");
  }

  SECTION("Invalid username: min") {
    std::string username = "usernam";
    auto expected_user = entity::make_user(username);
    REQUIRE(!expected_user);

    auto& log = expected_user.error();
    REQUIRE(log.errors.size() == 1);
    REQUIRE(log.errors[0] == "username: min");
  }

  SECTION("Invalid username: max") {
    std::string username{};
    for (int i = 0; i < USERNAME_OVER_MAX; i++) {
      username += 'a';
    }

    auto expected_user = entity::make_user(username);
    REQUIRE(!expected_user);

    auto& log = expected_user.error();
    REQUIRE(log.errors.size() == 1);
    REQUIRE(log.errors[0] == "username: max");
  }

  SECTION("Invalid password: min") {
    std::string username = "username";
    auto expected_user = entity::make_user(username);
    REQUIRE(expected_user);

    auto& user = expected_user.value();

    std::string password = "passwor";
    auto log = user->set_password(password);
    REQUIRE(log != std::nullopt);
    REQUIRE(log->errors.size() == 1);
    REQUIRE(log->errors[0] == "password: min");
  }

  SECTION("Invalid password: max") {
    std::string username = "username";
    auto expected_user = entity::make_user(username);
    REQUIRE(expected_user);

    auto& user = expected_user.value();
    std::string password{};
    for (int i = 0; i < PASSWORD_OVER_MAX; i++) {
      password += 'a';
    }
    
    auto log = user->set_password(password);
    REQUIRE(log != std::nullopt);
    REQUIRE(log->errors.size() == 1);
    REQUIRE(log->errors[0] == "password: max");
  }
}
