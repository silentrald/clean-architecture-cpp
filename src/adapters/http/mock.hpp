#ifndef HTTP_ADAPTER_MOCK_HPP
#define HTTP_ADAPTER_MOCK_HPP

#include "./build.hpp"
#include "entities/log/main.hpp"
#include "entities/user/factory.hpp"
#include "tl/expected.hpp"
#include <memory>

namespace adapter {

struct MockRequestConfig {
  std::string body = "{}";
  std::string session_id;
  std::optional<entity::User> user = std::nullopt;
  bool auth;
  bool session_cleared;
};

template <typename Body>
class MockRequest : public IRequest<Body, MockRequest<Body>> {
private:
  MockRequestConfig config;

public:
  explicit MockRequest(
      const MockRequestConfig& config = {.body = "{}", .user = std::nullopt}
  )
      : config(config) {}

  [[nodiscard]] tl::expected<Body, entity::Log> get_body_impl() noexcept;

  [[nodiscard]] bool is_auth_impl() noexcept {
    return this->config.auth;
  }

  [[nodiscard]] std::string get_session_id_impl() noexcept {
    return this->config.session_id;
  }

  std::unique_ptr<entity::User> get_session_user_impl() noexcept {
    if (this->config.user) {
      return std::make_unique<entity::User>(*this->config.user);
    }

    return nullptr;
  }

  std::optional<entity::Log> set_session_user_impl(entity::User* user
  ) noexcept {
    return std::nullopt;
  }

  bool clear_session_user_impl() noexcept {
    return this->config.session_cleared;
  }
};

class MockResponse : public IResponse<MockResponse> {
public:
  ContentType content_type = ContentType::text_html;
  ResponseStatus status = ResponseStatus::ok;
  std::string body; // NOTE: Make this a generic

  MockResponse() = default;

  void set_content_type_impl(const ContentType& type) noexcept;
  template <typename Body> void set_body_impl(const Body& body) noexcept;
  void set_status_impl(const ResponseStatus& status) noexcept;
};

} // namespace adapter

#endif

