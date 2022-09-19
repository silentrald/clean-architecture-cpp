#include "api/auth.hpp"
#include "interfaces/logger/singleton.hpp"
#include "interfaces/store/singleton.hpp"
#include "use-cases/auth/singleton.hpp"
#include "utils/env.hpp"
#include "wrappers/http/server.hpp"
#include <string>

int main() {
  utils::env::init_env();

  const int BASE = 10;

  auto err = db::init_postgres(
      {.host = utils::env::get_env("PG_HOST", "127.0.0.1"),
       .port = utils::env::get_env("PG_PORT", "5432"),
       .user = utils::env::get_env("PG_USER", "postgres"),
       .password = utils::env::get_env("PG_PASS", "password"),
       .db = utils::env::get_env("PG_DB", "postgres"),
       .pool = static_cast<uint>(std::strtoul(
           utils::env::get_env("PG_POOL", "1").c_str(), nullptr, BASE
       ))}
  );
  if (err != std::nullopt) {
    interface::get_logger()->fatal(err->to_string());
    return -1;
  }

  err = interface::init_store(
      {.host = utils::env::get_env("REDIS_HOST", "127.0.0.1"),
       .port = static_cast<int>(std::strtol(
           utils::env::get_env("REDIS_PORT", "6379").c_str(), nullptr, BASE
       )),
       .pool = static_cast<uint>(std::strtoul(
           utils::env::get_env("REDIS_POOL", "1").c_str(), nullptr, BASE
       ))}
  );
  if (err != std::nullopt) {
    interface::get_logger()->fatal(err->to_string());
    return -1;
  }

  std::string HOST = utils::env::get_env("HOST", "127.0.0.1");
  std::string PORT = utils::env::get_env("PORT", "5000");
  std::string POOL_STRING = utils::env::get_env("POOL", "1");
  uint POOL = std::strtoul(POOL_STRING.c_str(), nullptr, BASE);

  // Initialize the apis here
  api::Auth auth_api;

  try {
    http::server::server s(HOST, PORT, auth_api.get_routers(), "/api", POOL);
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
