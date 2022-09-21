#include "./singleton.hpp"
#include "adapters/http/build.hpp"
#include "adapters/http/mock.hpp"
#include "db/user/mock.hpp"
#include "entities/log/main.hpp"
#include "entities/user/factory.hpp"
#include "interfaces/logger/silent.hpp"
#include "interfaces/store/mock.hpp"
#include "tl/expected.hpp"
#include "use-cases/auth/login.hpp"
#include <catch2/catch_message.hpp>
#include <catch2/catch_test_macros.hpp>
#include <memory>

interface::SilentLogger logger;
use_case::LogoutAuthUseCase<interface::SilentLogger> logout_auth{&logger};

// NOLINTNEXTLINE
TEST_CASE("Login Auth Use Case", "Use Case") {
  SECTION("Successful Login", "json") {
    std::string id = "some-id";
    std::string username = "username";
    std::string password = "password";
    std::string hashed = *entity::UserCrypto::hash(password);

    auto user = std::make_shared<entity::User>(id, username, hashed);
    db::MockUser user_db({.user = user});
    interface::MockStore store({});
    use_case::LoginAuthUseCase<
        db::MockUser, interface::MockStore, interface::SilentLogger>
        login_auth{&user_db, &store, &logger};

    std::string body = R"({"username": "username", "password": "password"})";
    adapter::MockRequest<adapter::json> req({.body = body});
    adapter::MockResponse res{};

    login_auth.execute(req, res);
    REQUIRE(res.status == adapter::ResponseStatus::no_content);
  }

  SECTION("Already Authenticated", "json") {
    use_case::LoginAuthUseCase<
        db::MockUser, interface::MockStore, interface::SilentLogger>
        login_auth{nullptr, nullptr, nullptr};

    std::string body = R"({"username": "username", "password": "password"})";
    adapter::MockRequest<adapter::json> req({.body = body, .auth = true});
    adapter::MockResponse res{};

    login_auth.execute(req, res);
    REQUIRE(res.status == adapter::ResponseStatus::forbidden);
  }

  SECTION("No Body Passed", "json") {
    use_case::LoginAuthUseCase<
        db::MockUser, interface::MockStore, interface::SilentLogger>
        login_auth{nullptr, nullptr, nullptr};

    adapter::MockRequest<adapter::json> req{};
    adapter::MockResponse res{};

    login_auth.execute(req, res);
    REQUIRE(res.status == adapter::ResponseStatus::unauthorized);
  }

  SECTION("No username/password passed", "json") {
    use_case::LoginAuthUseCase<
        db::MockUser, interface::MockStore, interface::SilentLogger>
        login_auth{nullptr, nullptr, nullptr};

    std::string body1 = R"({"password": "password"})";
    adapter::MockRequest<adapter::json> req1({.body = body1});
    adapter::MockResponse res1{};

    login_auth.execute(req1, res1);
    REQUIRE(res1.status == adapter::ResponseStatus::unauthorized);

    std::string body2 = R"({"username": "username"})";
    adapter::MockRequest<adapter::json> req2({.body = body2});
    adapter::MockResponse res2{};

    login_auth.execute(req2, res2);
    REQUIRE(res2.status == adapter::ResponseStatus::unauthorized);
  }

  SECTION("Empty username/password", "json") {
    use_case::LoginAuthUseCase<
        db::MockUser, interface::MockStore, interface::SilentLogger>
        login_auth{nullptr, nullptr, nullptr};

    std::string body1 = R"({"username": "username", "password": ""})";
    adapter::MockRequest<adapter::json> req1({.body = body1});
    adapter::MockResponse res1{};

    login_auth.execute(req1, res1);
    REQUIRE(res1.status == adapter::ResponseStatus::unauthorized);

    std::string body2 = R"({"username": "", "password": "password"})";
    adapter::MockRequest<adapter::json> req2({.body = body2});
    adapter::MockResponse res2{};

    login_auth.execute(req2, res2);
    REQUIRE(res2.status == adapter::ResponseStatus::unauthorized);
  }

  SECTION("User Not Found", "json") {
    auto user = tl::unexpected<entity::Log>({.msg = "asdf"});
    db::MockUser user_db({.user = user});
    interface::MockStore store({});
    use_case::LoginAuthUseCase<
        db::MockUser, interface::MockStore, interface::SilentLogger>
        login_auth{&user_db, &store, &logger};

    std::string body = R"({"username": "username", "password": "password"})";
    adapter::MockRequest<adapter::json> req({.body = body});
    adapter::MockResponse res{};

    login_auth.execute(req, res);
    REQUIRE(res.status == adapter::ResponseStatus::unauthorized);
  }

  SECTION("Password Mismatch", "json") {
    std::string id = "some-id";
    std::string username = "username";
    std::string password = "password";
    std::string hashed = *entity::UserCrypto::hash(password);

    auto user = std::make_shared<entity::User>(id, username, hashed);
    db::MockUser user_db({.user = user});
    interface::MockStore store({});
    use_case::LoginAuthUseCase<
        db::MockUser, interface::MockStore, interface::SilentLogger>
        login_auth{&user_db, &store, &logger};

    std::string body =
        R"({"username": "username", "password": "some-random-password"})";
    adapter::MockRequest<adapter::json> req({.body = body});
    adapter::MockResponse res{};

    login_auth.execute(req, res);
    REQUIRE(res.status == adapter::ResponseStatus::unauthorized);
  }
}

// NOLINTNEXTLINE
TEST_CASE("Logout Auth Use Case", "Use Case") {
  SECTION("Successful Logout", "json") {
    adapter::MockRequest<adapter::json> req(
        {.auth = true, .session_cleared = true}
    );
    adapter::MockResponse res{};

    logout_auth.execute(req, res);
    REQUIRE(res.status == adapter::ResponseStatus::no_content);
  }

  SECTION("Not Authenticated", "json") {
    adapter::MockRequest<adapter::json> req({.auth = false});
    adapter::MockResponse res{};

    logout_auth.execute(req, res);
    REQUIRE(res.status == adapter::ResponseStatus::unauthorized);
  }

  SECTION("Could not clear session", "json") {
    std::string username = "username";
    auto user = entity::make_user(username);
    adapter::MockRequest<adapter::json> req(
        {.user = **user, .auth = true, .session_cleared = false}
    );
    adapter::MockResponse res{};

    logout_auth.execute(req, res);
    REQUIRE(res.status == adapter::ResponseStatus::internal_server_error);
  }

  SECTION("No Segmentation error if no user is found", "json") {
    adapter::MockRequest<adapter::json> req(
        {.auth = true, .session_cleared = false}
    );
    adapter::MockResponse res{};

    logout_auth.execute(req, res);
    REQUIRE(res.status == adapter::ResponseStatus::internal_server_error);
  }
}

