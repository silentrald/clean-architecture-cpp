#ifndef HTTP_ADAPTER_BUILD_HPP
#define HTTP_ADAPTER_BUILD_HPP

#include "entities/log/main.hpp"
#include "entities/user/factory.hpp"
#include "nlohmann/json.hpp"
#include "tl/expected.hpp"
#include <memory>
#include <optional>

namespace adapter {

using json = nlohmann::json;

enum class ResponseStatus {
  // 2xx
  ok = 200,
  created = 201,
  no_content = 204,

  // 4xx
  bad_request = 400,
  unauthorized = 401,
  forbidden = 403,
  not_found = 404,
  /* method_not_allowed = 405 */

  // 5xx
  internal_server_error = 500,
};

enum class ContentType { application_json, text_html };

// Factory
template <typename T> class IRequest {
private:
  IRequest() = default;

public:
  [[nodiscard]] std::string get_content_type() noexcept {
    return static_cast<T*>(this)->get_content_type_impl();
  }

  [[nodiscard]] std::string get_body() noexcept {
    return static_cast<T*>(this)->get_body_impl();
  }

  [[nodiscard]] bool is_auth() noexcept {
    return static_cast<T*>(this)->is_auth_impl();
  }

  [[nodiscard]] std::string get_session_id() noexcept {
    return static_cast<T*>(this)->get_session_id_impl();
  }

  std::optional<entity::Log> set_session_user(entity::User* user) noexcept {
    return static_cast<T*>(this)->set_session_user_impl(user);
  }

  std::unique_ptr<entity::User> get_session_user() noexcept {
    return static_cast<T*>(this)->get_session_user_impl();
  }

  bool clear_session_user() noexcept {
    return static_cast<T*>(this)->clear_session_user_impl();
  }

  friend T;

  [[nodiscard]] std::string get_content_type_impl() noexcept = delete;
  [[nodiscard]] std::string get_body_impl() noexcept = delete;
  [[nodiscard]] bool is_auth_impl() noexcept = delete;
  [[nodiscard]] std::string get_session_id_impl() noexcept = delete;
  std::optional<entity::User> get_session_user_impl() noexcept = delete;
  std::optional<entity::Log> set_session_user_impl(entity::User* user
  ) noexcept = delete;
  bool clear_session_user_impl() noexcept = delete;
};

// Builder
template <typename T> class IResponse {
private:
  IResponse() = default;

public:
  void set_content_type(const ContentType& type) noexcept {
    return static_cast<T*>(this)->set_content_type_impl(type);
  }

  template <typename Body> void set_body(const Body& body) noexcept {
    return static_cast<T*>(this)->set_body_impl(body);
  }

  void set_status(const ResponseStatus& status) noexcept {
    static_cast<T*>(this)->set_status_impl(status);
  }

  friend T;

  void set_content_type_impl(const ContentType& type) noexcept = delete;
  template <typename Body>
  void set_body_impl(const Body& body) noexcept = delete;
  void set_status_impl(const ResponseStatus& status) noexcept = delete;
};

} // namespace adapter

#endif

