#ifndef API_AUTH_HPP
#define API_AUTH_HPP

#include "adapters/http/factory.hpp"
#include "use-cases/auth/singleton.hpp"
#include "wrappers/http/request.hpp"
#include "wrappers/http/response.hpp"
#include "wrappers/http/router.hpp"
#include <memory>
#include <vector>

namespace api {

class Auth {
private:
  const http::server::router login_auth_rtr{
      .method = "POST",
      .path = "/auth/login",
      .cb = [](http::server::request* req,
               http::server::response* res) noexcept {
        adapter::WrapperRequest<adapter::json> areq(req, res);
        adapter::WrapperResponse ares(res);
        use_case::login_auth()->execute(areq, ares);
      }};

  const http::server::router logout_auth_rtr{
      .method = "POST",
      .path = "/auth/logout",
      .cb = [](http::server::request* req,
               http::server::response* res) noexcept {
        adapter::WrapperRequest<adapter::json> areq(req, res);
        adapter::WrapperResponse ares(res);
        use_case::logout_auth()->execute(areq, ares);
      }};

public:
  std::vector<http::server::router> get_routers() {
    return {login_auth_rtr, logout_auth_rtr};
  }
};

} // namespace api

#endif

