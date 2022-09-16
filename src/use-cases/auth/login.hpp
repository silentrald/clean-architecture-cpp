#ifndef LOGIN_USE_CASE_HTTP
#define LOGIN_USE_CASE_HTTP

#include "adapter/http/request.hpp"
#include "adapter/http/response.hpp"
#include "db/user/factory.hpp"
#include "interfaces/logger/factory.hpp"
#include "interfaces/store/singleton.hpp"
#include <exception>
#include <string>

namespace use_case {
class LoginAuthUseCase {
private:
  db::User* user_db;
  interface::Store* store;
  interface::Logger* logger;

public:
  LoginAuthUseCase(
      db::User* user_db, interface::Store* store, interface::Logger* logger
  )
      : user_db(user_db), store(store), logger(logger) {}

  void execute(adapter::request& req, adapter::response& res) noexcept {
    if (req.is_auth()) {
      res.status = adapter::response::forbidden;
      return;
    }

    auto username = req.get_body_val<std::string>("username");
    auto password = req.get_body_val<std::string>("password");
    if (!username || !password) {
      res.status = adapter::response::unauthorized;
      return;
    }

    auto user_exp = this->user_db->get_user_by_username(username.value());
    if (!user_exp) { // User does not exist
      this->logger->warn(user_exp.error().to_string());
      res.status = adapter::response::unauthorized;
      return;
    }
    auto user = *user_exp;
    
    auto same = user->verify_password(password.value());
    if (!same) {
      this->logger->warn(same.error().to_string());
      res.status = adapter::response::unauthorized;
      return;
    }

    if (!*same) {
      this->logger->warn(
          "Someone is trying to login to this account: " + username.value()
      );
      res.status = adapter::response::unauthorized;
      return;
    }

    req.set_session_user(user.get());

    res.status = adapter::response::no_content;
  }
};
} // namespace use_case

#endif

