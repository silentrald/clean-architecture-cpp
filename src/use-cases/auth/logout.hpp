#ifndef LOGOUT_USE_CASE_HTTP
#define LOGOUT_USE_CASE_HTTP

#include "adapters/http/factory.hpp"
#include "db/user/singleton.hpp"
#include "interfaces/logger/build.hpp"
#include "interfaces/logger/dev.hpp"
#include "interfaces/logger/singleton.hpp"
#include "interfaces/store/singleton.hpp"
#include <exception>
#include <optional>
#include <string>

namespace use_case {
template <typename Logger = interface::DefLogger> class LogoutAuthUseCase {
private:
  interface::ILogger<Logger>* logger;

public:
  explicit LogoutAuthUseCase(interface::ILogger<Logger>* logger)
      : logger(logger){};

  template <typename Req, typename Res>
  void execute(
      adapter::IRequest<std::nullopt_t, Req>& req, adapter::IResponse<Res>& res
  ) noexcept {
    if (!req.is_auth()) {
      res.set_status(adapter::ResponseStatus::unauthorized);
      return;
    }

    bool logout = req.clear_session_user();
    if (!logout) {
      auto user = req.get_session_user();

      // Avoids sigsegv
      if (user != nullptr) {
        this->logger->error("Could not logout " + user->get_username());
      }

      res.set_status(adapter::ResponseStatus::internal_server_error);
      return;
    }

    res.set_status(adapter::ResponseStatus::no_content);
  }
};
} // namespace use_case

#endif

