#ifndef LOGIN_AUTH_USE_CASE_HTTP
#define LOGIN_AUTH_USE_CASE_HTTP

#include "adapters/http/build.hpp"
#include "adapters/http/factory.hpp"
#include "adapters/http/wrapper.hpp"
#include "db/user/singleton.hpp"
#include "entities/log/main.hpp"
#include "fbs/auth.hpp"
#include "interfaces/logger/build.hpp"
#include "interfaces/logger/singleton.hpp"
#include "interfaces/store/redis.hpp"
#include "interfaces/store/singleton.hpp"
#include "nlohmann/json.hpp"
#include "tl/expected.hpp"
#include <cstdint>
#include <exception>
#include <flatbuffers/buffer.h>
#include <flatbuffers/string.h>
#include <flatbuffers/verifier.h>
#include <string>

namespace use_case {

struct LoginAuthBody {
  std::string username;
  std::string password;
};

template <
    typename UserDb = db::DefUser, typename Store = interface::DefStore,
    typename Logger = interface::DefLogger>
class LoginAuthUseCase {
private:
  db::IUser<UserDb>* user_db;
  interface::IStore<Store>* store;
  interface::ILogger<Logger>* logger;

  template <typename Req>
  tl::expected<LoginAuthBody, entity::Log>
  parse_body(adapter::IRequest<Req>& req) noexcept {
    std::string body = req.get_body();
    if (body.empty()) {
      return tl::unexpected<entity::Log>({.msg = "Empty Body"});
    }

    if (req.get_content_type() == "application/flatbuffers") {
      flatbuffers::Verifier verifier{(uint8_t*)body.data(), body.size()};
      const auto* buffer = flatbuffers::GetRoot<fb::LoginRequest>(body.data());
      if (!buffer->Verify(verifier)) {
        return tl::unexpected<entity::Log>({.msg = "Invalid body"});
      }

      return LoginAuthBody{
          .username = buffer->username()->c_str(),
          .password = buffer->password()->c_str()};
    }

    try {
      auto json = nlohmann::json::parse(body);
      return LoginAuthBody{
          .username = json["username"], .password = json["password"]};
    } catch (std::exception& err) {
      return tl::unexpected<entity::Log>({.msg = "Invalid body"});
    }
  }

public:
  LoginAuthUseCase(
      db::IUser<UserDb>* user_db, interface::IStore<Store>* store,
      interface::ILogger<Logger>* logger
  )
      : user_db(user_db), store(store), logger(logger) {}

  template <typename Req, typename Res>
  void
  execute(adapter::IRequest<Req>& req, adapter::IResponse<Res>& res) noexcept {
    if (req.is_auth()) {
      res.set_status(adapter::ResponseStatus::forbidden);
      return;
    }

    auto body = this->parse_body(req);
    if (!body || body->username.empty() || body->password.empty()) {
      res.set_status(adapter::ResponseStatus::unauthorized);
      return;
    }

    auto user_exp = this->user_db->get_user_by_username(body->username);
    if (!user_exp) { // User does not exist
      this->logger->warn(user_exp.error().to_string());
      res.set_status(adapter::ResponseStatus::unauthorized);
      return;
    }
    auto user = *user_exp;

    auto same = user->verify_password(body->password);
    if (!same) {
      this->logger->warn(same.error().to_string());
      res.set_status(adapter::ResponseStatus::unauthorized);
      return;
    }

    if (!*same) {
      this->logger->warn(
          "Someone is trying to login to this account: " + body->username
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

