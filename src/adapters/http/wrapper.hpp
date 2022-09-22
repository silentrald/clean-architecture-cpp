#ifndef HTTP_ADAPTER_WRAPPER_HPP
#define HTTP_ADAPTER_WRAPPER_HPP

#include "adapters/http/build.hpp"
#include "asio/detail/chrono.hpp"
#include "asio/high_resolution_timer.hpp"
#include "entities/log/main.hpp"
#include "entities/user/factory.hpp"
#include "interfaces/logger/build.hpp"
#include "interfaces/logger/singleton.hpp"
#include "interfaces/store/singleton.hpp"
#include "tl/expected.hpp"
#include "wrappers/http/cookie.hpp"
#include "wrappers/http/request.hpp"
#include "wrappers/http/response.hpp"
#include <chrono>
#include <exception>
#include <memory>
#include <nlohmann/json.hpp>
#include <string>
#include <sys/random.h> // NOTE: Can be changed to libsodium
#include <unordered_map>
#include <vector>

namespace adapter {

const int SESSION_ID_LENGTH = 16;
const char* const ALPHABET =
    "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
const size_t ALPHABET_LEN = strlen(ALPHABET);
const char* const SESSION_PREFIX = "sess:";
const char* const SESSION_NAME = "SESSION";

template <typename Body, typename Store = interface::DefStore, typename Logger = interface::DefLogger>
class WrapperRequest : public IRequest<Body, WrapperRequest<Body, Store, Logger>> {
private:
  http::server::request* req;
  http::server::response* res;
  interface::IStore<Store>* store;
  interface::ILogger<Logger>* logger;
  json body;
  std::chrono::system_clock::time_point start;

  [[nodiscard]] std::string get_session_key() noexcept;

public:
  WrapperRequest() = delete;
  WrapperRequest(const WrapperRequest&) = delete;
  WrapperRequest(WrapperRequest&&) = delete;
  WrapperRequest& operator=(const WrapperRequest&) = delete;
  WrapperRequest& operator=(WrapperRequest&&) = delete;

  ~WrapperRequest() {
    auto ms = duration_cast<std::chrono::milliseconds>(
        std::chrono::high_resolution_clock::now() - this->start
    );
    this->logger->info(
        req->method + " " + req->uri + " " + std::to_string(res->status) + " " +
        std::to_string(ms.count()) + "ms"
    );
  }

  explicit WrapperRequest(
      http::server::request* req, http::server::response* res,
      interface::IStore<Store>* store = interface::get_store<Store>(),
      interface::ILogger<Logger>* logger = interface::get_logger<Logger>()
  )
      : req(req), res(res), store(store), logger(logger) {
    this->start = std::chrono::high_resolution_clock::now();
  }

  [[nodiscard]] tl::expected<Body, entity::Log> get_body_impl() noexcept;

  /* [[nodiscard]] std::vector<std::string> get_params() const; */

  // Session Functions

  /**
   * Checks whether the user is authenticated
   */
  [[nodiscard]] bool is_auth_impl() noexcept;

  /**
   * Gets the session id of the user
   */
  [[nodiscard]] std::string get_session_id_impl() noexcept;

  /**
   * Sets the user session to the store
   */
  std::optional<entity::Log> set_session_user_impl(entity::User* user) noexcept;

  /**
   * Gets the user from the session store
   *
   * @returns user without the password/hash fields
   */
  std::unique_ptr<entity::User> get_session_user_impl() noexcept;

  /**
   * Clears the user from the session store
   *
   * @returns true if the session was cleared else false
   */
  bool clear_session_user_impl() noexcept;
};

template <typename Body, typename Store, typename Logger>
[[nodiscard]] tl::expected<Body, entity::Log>
WrapperRequest<Body, Store, Logger>::get_body_impl() noexcept {
  return tl::unexpected<entity::Log>({.msg = "Invalid"});
}

template <>
[[nodiscard]] tl::expected<json, entity::Log>
WrapperRequest<json, interface::DefStore, interface::DefLogger>::get_body_impl() noexcept;

template <typename Body, typename Store, typename Logger>
[[nodiscard]] std::string
WrapperRequest<Body, Store, Logger>::get_session_id_impl() noexcept {
  std::string& ref = this->req->session_id;
  if (!ref.empty()) {
    return ref;
  }

  std::vector<uint64_t> buffer{};
  buffer.reserve(SESSION_ID_LENGTH);
  // NOTE: Can be changed to libsodium
  uint bytes =
      getrandom(buffer.data(), SESSION_ID_LENGTH * sizeof(uint64_t), 0);
  for (uint i = 0; i < SESSION_ID_LENGTH; i++) {
    ref.push_back(ALPHABET[buffer[i] % ALPHABET_LEN]); // NOLINT
  }

  return ref;
}

template <typename Body, typename Store, typename Logger>
[[nodiscard]] bool WrapperRequest<Body, Store, Logger>::is_auth_impl() noexcept {
  return this->store->exists(this->get_session_key());
}

template <typename Body, typename Store, typename Logger>
[[nodiscard]] std::string
WrapperRequest<Body, Store, Logger>::get_session_key() noexcept {
  return SESSION_PREFIX + this->get_session_id_impl();
}

template <typename Body, typename Store, typename Logger>
std::optional<entity::Log>
WrapperRequest<Body, Store, Logger>::set_session_user_impl(entity::User* user) noexcept {
  json user_json = {
    {"id", user->get_id()},
    {"username", user->get_username()}
  };
  this->store->set_string(this->get_session_key(), user_json.dump());
  this->res->cookies.emplace_back(http::server::cookie{
      .name = "SESSION",
      .value = this->get_session_id_impl(),
      .path = "/",
      .same_site = "strict",
      .max_age = 3600,
      .secure = false,
      .http_only = false});
  return std::nullopt;
}

// TODO: Add support with flatbuffers
template <typename Body, typename Store, typename Logger>
std::unique_ptr<entity::User>
WrapperRequest<Body, Store, Logger>::get_session_user_impl() noexcept {
  auto user_json = this->store->get_string(this->get_session_key());
  if (!user_json || !*user_json) {
    return nullptr;
  }

  json user = json::parse(**user_json);
  try {
    std::string id = user.at("id");
    std::string username = user.at("username");
    return std::make_unique<entity::User>(id, username);
  } catch (std::exception& e) {
    return nullptr;
  }
}

template <typename Body, typename Store, typename Logger>
bool WrapperRequest<Body, Store, Logger>::clear_session_user_impl() noexcept {
  return this->store->del(this->get_session_key());
}

//// RESPONSE ////
class WrapperResponse : public IResponse<WrapperResponse> {
private:
  http::server::response* res;

public:
  WrapperResponse() = delete;
  WrapperResponse(const WrapperResponse&) = delete;
  WrapperResponse(WrapperResponse&&) = delete;
  WrapperResponse& operator=(const WrapperResponse&) = delete;
  WrapperResponse& operator=(WrapperResponse&&) = delete;
  ~WrapperResponse() = default;

  explicit WrapperResponse(http::server::response* res) : res(res) {}

  void set_content_type_impl(const ContentType& type) noexcept;
  template <typename Body> void set_body_impl(const Body& body) noexcept;
  void set_status_impl(const ResponseStatus& status) noexcept;
};

} // namespace adapter

#endif

