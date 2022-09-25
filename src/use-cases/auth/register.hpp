#ifndef REGISTER_AUTH_USE_CASE_HTTP
#define REGISTER_AUTH_USE_CASE_HTTP

#include "adapters/http/build.hpp"
#include "db/user/build.hpp"
#include "db/user/singleton.hpp"
#include "entities/log/main.hpp"
#include "entities/user/factory.hpp"
#include "interfaces/logger/build.hpp"
#include "interfaces/logger/singleton.hpp"
#include "tl/expected.hpp"
#include <exception>
#include <memory>

namespace use_case {
struct RegisterAuthBody {
  std::string username;
  std::string password;
};

template <typename UserDb = db::DefUser, typename Logger = interface::DefLogger>
class RegisterAuthUseCase {
private:
  db::IUser<UserDb>* user_db;
  interface::ILogger<Logger>* logger;

  template <typename Req>
  tl::expected<RegisterAuthBody, entity::Log>
  parse_body(adapter::IRequest<Req>& req) noexcept {
    std::string body = req.get_body();
    if (body.empty()) {
      return tl::unexpected<entity::Log>({.msg = "Empty Body"});
    }

    // TODO: Flatbuffers

    try {
      auto json = nlohmann::json::parse(body);
      return RegisterAuthBody{
          .username = json["username"], .password = json["password"]};
    } catch (std::exception& err) {
      return tl::unexpected<entity::Log>({.msg = "Invalid body"});
    }
  }

public:
  RegisterAuthUseCase(
      db::IUser<UserDb>* user_db, interface::ILogger<Logger>* logger
  )
      : user_db(user_db), logger(logger) {}

  template <typename Req, typename Res>
  void
  execute(adapter::IRequest<Req>& req, adapter::IResponse<Res>& res) noexcept {
    if (req.is_auth()) {
      res.set_status(adapter::ResponseStatus::forbidden);
      return;
    }

    auto body = this->parse_body(req);
    if (!body) {
      res.set_body(body.error().msg);
      res.set_status(adapter::ResponseStatus::bad_request);
      return;
    }

    if (body->username.empty() || body->password.empty()) {
      res.set_body("Empty username/password");
      res.set_status(adapter::ResponseStatus::bad_request);
      return;
    }

    auto exp_user = entity::make_user(body->username);
    if (!exp_user) {
      res.set_body(exp_user.error().errors);
      res.set_status(adapter::ResponseStatus::bad_request);
      return;
    }

    auto user = *exp_user;
    std::optional<entity::Log> log = user->set_password(body->password);
    if (log) {
      res.set_body("Invalid password");
      res.set_status(adapter::ResponseStatus::bad_request);
      return;
    }

    log = user->hash_password();
    if (log) {
      this->logger->error(log->to_string());
      res.set_body("Could not hash");
      res.set_status(adapter::ResponseStatus::bad_request);
      return;
    }

    auto id = user_db->add_user(user.get());
    if (!id) {
      // User might already exist
      auto& error = id.error();
      if (error.db_contraint == "users_username_key") {
        res.set_body("Username already taken");
      } else {
        res.set_body(id.error().msg);
      }
      res.set_status(adapter::ResponseStatus::bad_request);
      return;
    }

    res.set_body(*id);
    res.set_status(adapter::ResponseStatus::created);
  }
};
} // namespace use_case

#endif

