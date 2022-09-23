#ifndef USER_AUTH_USE_CASE_HTTP
#define USER_AUTH_USE_CASE_HTTP

#include "adapters/http/build.hpp"
#include "entities/user/factory.hpp"
#include "interfaces/logger/build.hpp"
#include "interfaces/logger/singleton.hpp"
#include <memory>
#include <optional>

namespace use_case {

template <typename Logger = interface::DefLogger> class UserAuthUseCase {
private:
  interface::ILogger<Logger>* logger;

public:
  explicit UserAuthUseCase(interface::ILogger<Logger>* logger)
      : logger(logger) {}

  // TODO: Support flatbuffers
  template <typename Req, typename Res>
  void execute(
      adapter::IRequest<Req>& req, adapter::IResponse<Res>& res
  ) noexcept {
    if (!req.is_auth()) {
      res.set_status(adapter::ResponseStatus::unauthorized);
      return;
    }

    std::unique_ptr<entity::User> user = req.get_session_user();
    if (user == nullptr) {
      res.set_status(adapter::ResponseStatus::internal_server_error);
      return;
    }

    adapter::json body = {
        {"id", user->get_id()},
        {"username", user->get_username()},
    };
    res.set_body(body.dump());
    res.set_content_type(adapter::ContentType::application_json);
    res.set_status(adapter::ResponseStatus::ok);
  }
};

} // namespace use_case

#endif

