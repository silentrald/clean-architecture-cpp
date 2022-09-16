#include "adapter/http/request.hpp"
#include "db/core/singleton.hpp"
#include "entities/user/factory.hpp"
#include "interfaces/logger/factory.hpp"
#include "interfaces/store/singleton.hpp"
#include "use-cases/auth/factory.hpp"
#include "utils/env.hpp"
#include "wrappers/http/cookie.hpp"
#include "wrappers/http/header.hpp"
#include "wrappers/http/request.hpp"
#include "wrappers/http/response.hpp"
#include "wrappers/http/server.hpp"
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <optional>
#include <string>

int main() {
  utils::env::init_env();

  auto err = db::init_postgres(
      {.host = utils::env::get_env("PG_HOST", "127.0.0.1"),
       .port = utils::env::get_env("PG_PORT", "5432"),
       .user = utils::env::get_env("PG_USER", "postgres"),
       .password = utils::env::get_env("PG_PASS", "password"),
       .db = utils::env::get_env("PG_DB", "postgres"),
       .pool = static_cast<uint>(std::strtoul(
           utils::env::get_env("PG_POOL", "1").c_str(), nullptr, 10 // NOLINT
       ))}
  );
  if (err != std::nullopt) {
    interface::get_logger()->fatal(err->to_string());
    return -1;
  }

  err = interface::init_store(
      {.host = utils::env::get_env("REDIS_HOST", "127.0.0.1"),
       .port = static_cast<int>(std::strtol(
           utils::env::get_env("REDIS_PORT", "6379").c_str(), nullptr,
           10 // NOLINT
       )),
       .pool = static_cast<uint>(std::strtoul(
           utils::env::get_env("REDIS_POOL", "1").c_str(), nullptr, 10 // NOLINT
       ))}
  );
  if (err != std::nullopt) {
    interface::get_logger()->fatal(err->to_string());
    return -1;
  }

  const char* const HOST = "127.0.0.1";
  const char* const PORT = "5000";

  // TODO: Separate this to the api folder
  http::server::router login_auth_rtr{
      .method = "POST",
      .path = "/auth/login",
      .cb = [](http::server::request* req, http::server::response* res) {
        adapter::request areq{req, res};
        use_case::login_auth()->execute(areq, *res);
      }};

  // TODO: Separate this to the api folder
  http::server::router logout_auth_rtr{
      .method = "POST",
      .path = "/auth/logout",
      .cb = [](http::server::request* req, http::server::response* res) {
        adapter::request areq{req, res};
        use_case::logout_auth()->execute(areq, *res);
      }}; 

  try {
    http::server::server s(
        HOST, PORT, {login_auth_rtr, logout_auth_rtr}, "/api", 2
    );
    interface::get_logger()->info(
        std::string{"Running server http://"} + HOST + ":" + PORT
    );
    s.run();

    std::cout << "\nGraceful Stop\n";
  } catch (std::exception& e) {
    std::cerr << "exception: " << e.what() << "\n";
  }

  return 0;
}
