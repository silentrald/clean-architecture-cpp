#ifndef REQUEST_HTTP_ADAPTER_HPP
#define REQUEST_HTTP_ADAPTER_HPP

#include "asio/detail/chrono.hpp"
#include "asio/high_resolution_timer.hpp"
#include "entities/user/factory.hpp"
#include "interfaces/store/singleton.hpp"
#include "wrappers/http/cookie.hpp"
#include "wrappers/http/request.hpp"
#include "wrappers/http/response.hpp"
#include <chrono>
#include <exception>
#include <memory>
#include <nlohmann/json.hpp>
#include <string>
#include <unordered_map>
#include <vector>

namespace adapter {

using json = nlohmann::json;

class request {
private:
  http::server::request* req;
  http::server::response* res;
  json body;
  std::chrono::system_clock::time_point start;

  const int LENGTH = 16;
  const std::string ALPHABET =
      "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
  const std::string SESSION_PREFIX = "sess:";
  const std::string COOKIE_NAME = "SESSION";

  [[nodiscard]] std::string get_session_key();

public:
  request() = delete;
  request(const request&) = delete;
  request(request&&) = delete;
  request& operator=(const request&) = delete;
  request& operator=(request&&) = delete;
  ~request();

  explicit request(http::server::request* req, http::server::response* res)
      : req(req), res(res) {
    start = std::chrono::high_resolution_clock::now();
  }

  template <typename T>
  [[nodiscard]] std::optional<T> get_body_val(const std::string& key) noexcept {
    return this->get_body_val<T>(key.c_str());
  }

  template <typename T>
  [[nodiscard]] std::optional<T> get_body_val(const char* key) noexcept {
    if (this->body == nullptr) {
      if (this->req->body.empty()) {
        this->body = json::parse("{}");
      } else {
        this->body = json::parse(this->req->body);
      }
    }

    try {
      return this->body.at(key);
    } catch (std::exception& e) {
      return std::nullopt;
    }
  }

  /* [[nodiscard]] std::vector<std::string> get_params() const; */

  // Session Functions

  /**
   * Checks whether the user is authenticated
   */
  [[nodiscard]] bool is_auth();

  /**
   * Gets the session id of the user
   */
  [[nodiscard]] const std::string& get_session_id();

  /**
   * Sets the user session to the store
   */
  void set_session_user(entity::User* user);

  /**
   * Gets the user from the session store
   *
   * @returns user without the password/hash fields
   */
  std::unique_ptr<entity::User> get_session_user();

  /**
   * Clears the user from the session store
   *
   * @returns true if the session was cleared else false
   */
  bool clear_session_user();
};
} // namespace adapter

#endif

