#include "adapters/http/request.hpp"
#include "use-cases/auth/singleton.hpp"
#include "wrappers/http/request.hpp"
#include "wrappers/http/response.hpp"
#include "wrappers/http/router.hpp"
#include <vector>

namespace api {

class Auth {
private:
  const http::server::router login_auth_rtr{
      .method = "POST",
      .path = "/auth/login",
      .cb = [](http::server::request* req, http::server::response* res) {
        adapter::request areq{req, res};
        use_case::login_auth()->execute(areq, *res);
      }};

  const http::server::router logout_auth_rtr{
      .method = "POST",
      .path = "/auth/logout",
      .cb = [](http::server::request* req, http::server::response* res) {
        adapter::request areq{req, res};
        use_case::logout_auth()->execute(areq, *res);
      }};

public:
  std::vector<http::server::router> get_routers() {
    return {login_auth_rtr, logout_auth_rtr};
  }
};

} // namespace api

