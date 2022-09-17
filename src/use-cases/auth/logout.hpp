#ifndef LOGOUT_USE_CASE_HTTP
#define LOGOUT_USE_CASE_HTTP

#include "adapters/http/request.hpp"
#include "adapters/http/response.hpp"
#include "db/user/singleton.hpp"
#include "interfaces/logger/singleton.hpp"
#include "interfaces/store/singleton.hpp"
#include <exception>
#include <string>

namespace use_case {
class LogoutAuthUseCase {
private:
  interface::Logger* logger;

public:
  explicit LogoutAuthUseCase(interface::Logger* logger) : logger(logger) {}

  void execute(adapter::request& req, adapter::response& res) noexcept {
    if (!req.is_auth()) {
      res.status = adapter::response::forbidden;
      return;
    }

    bool logout = req.clear_session_user();
    if (!logout) {
      this->logger->error(
          "Could not logout " + req.get_session_user()->get_username()
      );
      res.status = adapter::response::internal_server_error;
      return;
    }

    res.status = adapter::response::no_content;
  }
};
} // namespace use_case

#endif

