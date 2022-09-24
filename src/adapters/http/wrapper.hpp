#ifndef HTTP_ADAPTER_WRAPPER_HPP
#define HTTP_ADAPTER_WRAPPER_HPP

#include "adapters/http/build.hpp"
#include "entities/log/main.hpp"
#include "entities/user/factory.hpp"
#include "fbs/auth.hpp"
#include "interfaces/logger/build.hpp"
#include "interfaces/logger/singleton.hpp"
#include "interfaces/store/singleton.hpp"
#include "tl/expected.hpp"
#include "utils/string.hpp"
#include "wrappers/http/cookie.hpp"
#include "wrappers/http/request.hpp"
#include "wrappers/http/response.hpp"
#include <chrono>
#include <cstdint>
#include <exception>
#include <flatbuffers/buffer.h>
#include <flatbuffers/flatbuffer_builder.h>
#include <flatbuffers/table.h>
#include <flatbuffers/verifier.h>
#include <memory>
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

template <
    typename Store = interface::DefStore,
    typename Logger = interface::DefLogger>
class WrapperRequest : public IRequest<WrapperRequest<Store, Logger>> {
private:
  http::server::request* req;
  http::server::response* res;
  interface::IStore<Store>* store;
  interface::ILogger<Logger>* logger;
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

  [[nodiscard]] std::string get_content_type_impl() noexcept;
  [[nodiscard]] std::string get_body_impl() noexcept;

  /* [[nodiscard]] std::vector<std::string> get_params() const; */

  // Session Functions

  [[nodiscard]] bool is_auth_impl() noexcept;
  [[nodiscard]] std::string get_session_id_impl() noexcept;
  [[nodiscard]] std::string get_new_session_id_impl() noexcept;
  std::optional<entity::Log> set_session_user_impl(entity::User* user) noexcept;
  std::unique_ptr<entity::User> get_session_user_impl() noexcept;
  bool clear_session_user_impl() noexcept;
};

template <typename Store, typename Logger>
[[nodiscard]] std::string
WrapperRequest<Store, Logger>::get_content_type_impl() noexcept {
  return this->req->content_type;
}

template <typename Store, typename Logger>
[[nodiscard]] std::string
WrapperRequest<Store, Logger>::get_body_impl() noexcept {
  return this->req->body;
}

template <typename Store, typename Logger>
[[nodiscard]] std::string
WrapperRequest<Store, Logger>::get_session_id_impl() noexcept {
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

// TODO: Add a fail case if the user cant create a new session id
template <typename Store, typename Logger>
[[nodiscard]] std::string
WrapperRequest<Store, Logger>::get_new_session_id_impl() noexcept {
  std::string& ref = this->req->session_id;
  std::vector<uint64_t> buffer{};
  buffer.reserve(SESSION_ID_LENGTH);

  do {
    ref = "";
    // NOTE: Can be changed to libsodium
    uint bytes =
        getrandom(buffer.data(), SESSION_ID_LENGTH * sizeof(uint64_t), 0);
    for (uint i = 0; i < SESSION_ID_LENGTH; i++) {
      ref.push_back(ALPHABET[buffer[i] % ALPHABET_LEN]); // NOLINT
    }
    // Collision Check
  } while (this->store->exists(this->get_session_key()));
  
  return ref;
}

template <typename Store, typename Logger>
[[nodiscard]] bool WrapperRequest<Store, Logger>::is_auth_impl() noexcept {
  return this->store->exists(this->get_session_key());
}

template <typename Store, typename Logger>
[[nodiscard]] std::string
WrapperRequest<Store, Logger>::get_session_key() noexcept {
  return SESSION_PREFIX + this->get_session_id_impl();
}

template <typename Store, typename Logger>
std::optional<entity::Log>
WrapperRequest<Store, Logger>::set_session_user_impl(entity::User* user
) noexcept {
  flatbuffers::FlatBufferBuilder builder(256);
  auto id = builder.CreateString(user->get_id());
  auto username = builder.CreateString(user->get_username());
  auto offset = fb::CreateUser(builder, id, username);
  builder.Finish(offset);

  const auto* sample =
      flatbuffers::GetRoot<fb::User>(builder.GetBufferPointer());

  std::string sid = this->get_new_session_id_impl();
  this->store->set_byte_array(
      this->get_session_key(), builder.GetBufferPointer(), builder.GetSize()
  );
  this->res->cookies.emplace_back(http::server::cookie{
      .name = SESSION_NAME,
      .value = sid,
      .path = "/",
      .same_site = "strict",
      .max_age = 3600,
      .secure = false,
      .http_only = false});
  return std::nullopt;
}

template <typename Store, typename Logger>
std::unique_ptr<entity::User>
WrapperRequest<Store, Logger>::get_session_user_impl() noexcept {
  auto data = this->store->get_byte_array(this->get_session_key());
  if (!data || !*data) {
    return nullptr;
  }

  std::vector<uint8_t>& user_data = **data;
  flatbuffers::Verifier verifier{user_data.data(), user_data.size()};
  const auto* user = flatbuffers::GetRoot<fb::User>(user_data.data());

  if (!user->Verify(verifier)) {
    this->logger->debug("Invalid data");
    return nullptr;
  }

  std::string id = user->id()->c_str();
  std::string username = user->username()->c_str();
  return std::make_unique<entity::User>(id, username);
}

template <typename Store, typename Logger>
bool WrapperRequest<Store, Logger>::clear_session_user_impl() noexcept {
  bool cleared = this->store->del(this->get_session_key());
  if (cleared) {
    this->res->cookies.push_back(
        {.name = SESSION_NAME,
         .path = "/",
         .same_site = "strict",
         .max_age = 0,
         .secure = false,
         .http_only = false}
    );
  }
  return cleared;
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

