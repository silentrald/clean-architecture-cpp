#include "./request.hpp"
#include <sys/random.h>

namespace adapter {

request::~request() {
  auto ms = duration_cast<std::chrono::milliseconds>(
      std::chrono::high_resolution_clock::now() - this->start
  );
  interface::get_logger()->info(
      req->method + " " + req->uri + " " + std::to_string(res->status) + " " +
      std::to_string(ms.count()) + "ms"
  );
}

[[nodiscard]] const std::string& request::get_session_id() {
  std::string& ref = this->req->session_id;
  if (!ref.empty()) {
    return ref;
  }

  std::vector<uint64_t> buffer{};
  buffer.reserve(this->LENGTH);
  uint bytes = getrandom(buffer.data(), this->LENGTH * sizeof(uint64_t), 0);
  for (uint i = 0; i < this->LENGTH; i++) {
    ref.push_back(this->ALPHABET[buffer[i] % this->ALPHABET.size()]);
  }

  return ref;
}

[[nodiscard]] bool request::is_auth() {
  return interface::get_store()->exists(this->get_session_key());
}

std::string request::get_session_key() {
  return this->SESSION_PREFIX + this->get_session_id();
}

void request::set_session_user(entity::User* user) {
  // TODO: Dependency Injection
  std::string key = this->get_session_key();
  interface::get_store()->set_string(
      key, R"({"id": ")" + user->get_id() + R"(, "username": ")" +
               user->get_username() + R"(})"
  );
  this->res->cookies.emplace_back(http::server::cookie{
      .name = "SESSION",
      .value = key,
      .path = "/",
      .same_site = "strict",
      .max_age = 3600,
      .secure = false,
      .http_only = false});
}

std::unique_ptr<entity::User> request::get_session_user() {
  auto user_json = interface::get_store()->get_string(this->get_session_key());
  if (!user_json) {
    return nullptr;
  }

  json user = json::parse(user_json.value());
  try {
    std::string id = user.at("id");
    std::string username = user.at("username");
    return std::make_unique<entity::User>(id, username);
  } catch (std::exception& e) {
    return nullptr;
  }
}

bool request::clear_session_user() {
  return interface::get_store()->del(this->get_session_key());
}

} // namespace adapter
