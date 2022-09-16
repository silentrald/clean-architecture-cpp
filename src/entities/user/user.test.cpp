/* #include "./build.hpp" */
#include <catch2/catch_all.hpp>
/* #include <memory> */

/* const int USERNAME_OVER_MAX = 51; */
/* const int PASSWORD_OVER_MAX = 61; */

// NOLINTNEXTLINE (cert-err58-cpp)
TEST_CASE("Testing User Entity") {
  /*   SECTION("All fields") { */
  /*     std::string id = "id"; */
  /*     std::string username = "username"; */
  /*     auto expected_user = entity::make_user(id, username); */
  /*     REQUIRE(expected_user); */

  /*     auto& user = expected_user.value(); */
  /*     REQUIRE(user->get_id() == "id"); */
  /*     REQUIRE(user->get_username() == "username"); */

  /*     std::string password = "password"; */
  /*     auto error = user->set_password(password); */
  /*     REQUIRE(error); */

  /*     error = user->hash_password(); */
  /*     REQUIRE(error == 0); */

  /*     password = "password"; */
  /*     REQUIRE(user->compare_password(password)); */
  /*   } */

  /*   SECTION("Success with only username") { */
  /*     std::string username = "username"; */
  /*     auto expected_user = entity::make_user(username); */
  /*     REQUIRE(expected_user); */

  /*     auto& user = expected_user.value(); */
  /*     REQUIRE(user->get_id().empty()); */
  /*     REQUIRE(user->get_username() == "username"); */
  /*   } */

  /*   SECTION("Invalid min") { */
  /*     std::string username = "usernam"; */
  /*     auto expected_user = entity::make_user(username); */
  /*     REQUIRE(!expected_user); */

  /*     auto& errors = expected_user.error(); */
  /*     REQUIRE(errors[0] == "username: min"); */
  /*   } */

  /*   SECTION("Invalid max") { */
  /*     std::string username{}; */
  /*     for (int i = 0; i < USERNAME_OVER_MAX; i++) { */
  /*       username += 'a'; */
  /*     } */

  /*     auto expected_user = entity::make_user(username); */
  /*     REQUIRE(!expected_user); */

  /*     auto& errors = expected_user.error(); */
  /*     REQUIRE(errors[0] == "username: max"); */
  /*   } */

  /*   SECTION("Invalid min password") { */
  /*     std::string username = "username"; */
  /*     auto expected_user = entity::make_user(username); */
  /*     REQUIRE(expected_user); */

  /*     auto& user = expected_user.value(); */
  /*     std::string password = "passwor"; */
  /*     auto error = user->set_password(password); */
  /*     REQUIRE(!error); */
  /*     REQUIRE(error.error() == "password: min"); */
  /*   } */

  /*   SECTION("Invalid max password") { */
  /*     std::string username = "username"; */
  /*     auto expected_user = entity::make_user(username); */
  /*     REQUIRE(expected_user); */

  /*     auto& user = expected_user.value(); */
  /*     std::string password{}; */
  /*     for (int i = 0; i < PASSWORD_OVER_MAX; i++) { */
  /*       password += 'a'; */
  /*     } */
  /*     auto error = user->set_password(password); */
  /*     REQUIRE(!error); */
  /*     REQUIRE(error.error() == "password: max"); */
  /*   } */
}
