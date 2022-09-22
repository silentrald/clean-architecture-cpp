#ifndef LOGIN_AUTH_USE_CASE_HTTP
#define LOGIN_AUTH_USE_CASE_HTTP

#include "adapters/http/build.hpp"
#include "adapters/http/factory.hpp"
#include "adapters/http/wrapper.hpp"
#include "db/user/singleton.hpp"
#include "interfaces/logger/build.hpp"
#include "interfaces/logger/singleton.hpp"
#include "interfaces/store/redis.hpp"
#include "interfaces/store/singleton.hpp"
#include <exception>
#include <string>

namespace use_case {
template <
    typename UserDb = db::DefUser, typename Store = interface::DefStore,
    typename Logger = interface::DefLogger>
class LoginAuthUseCase {
private:
  db::IUser<UserDb>* user_db;
  interface::IStore<Store>* store;
  interface::ILogger<Logger>* logger;

public:
  LoginAuthUseCase(
      db::IUser<UserDb>* user_db, interface::IStore<Store>* store,
      interface::ILogger<Logger>* logger
  )
      : user_db(user_db), store(store), logger(logger) {}

  template <typename Req, typename Res>
  void execute(
      adapter::IRequest<adapter::json, Req>& req, adapter::IResponse<Res>& res
  ) noexcept {
    if (req.is_auth()) {
      res.set_status(adapter::ResponseStatus::forbidden);
      return;
    }

    auto exp_body = req.get_body();
    if (!exp_body) {
      res.set_status(adapter::ResponseStatus::unauthorized);
      return;
    }
    auto body = *exp_body;

    std::string username;
    std::string password;
    try {
      username = body["username"];
      password = body["password"];
    } catch (std::exception& err) {
      res.set_status(adapter::ResponseStatus::unauthorized);
      return;
    }

    if (username.empty() || password.empty()) {
      res.set_status(adapter::ResponseStatus::unauthorized);
      return;
    }

    auto user_exp = this->user_db->get_user_by_username(username);
    if (!user_exp) { // User does not exist
      this->logger->warn(user_exp.error().to_string());
      res.set_status(adapter::ResponseStatus::unauthorized);
      return;
    }
    auto user = *user_exp;

    auto same = user->verify_password(password);
    if (!same) {
      this->logger->warn(same.error().to_string());
      res.set_status(adapter::ResponseStatus::unauthorized);
      return;
    }

    if (!*same) {
      this->logger->warn(
          "Someone is trying to login to this account: " + username
      );
      res.set_status(adapter::ResponseStatus::unauthorized);
      return;
    }

    // TODO: May return an error
    req.set_session_user(user.get());

    res.set_status(adapter::ResponseStatus::no_content);
  }
};

} // namespace use_case

#endif

