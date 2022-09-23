#include "./wrapper.hpp"
#include "fbs/auth.hpp"
#include "interfaces/logger/dev.hpp"
#include "interfaces/logger/singleton.hpp"
#include "interfaces/store/singleton.hpp"
#include "tl/expected.hpp"
#include <cstring>
#include <exception>
#include <flatbuffers/buffer.h>
#include <flatbuffers/table.h>
#include <memory>
#include <optional>

namespace adapter {

template <>
[[nodiscard]] tl::expected<json*, entity::Log>
WrapperRequest<json, interface::DefStore, interface::DefLogger>::get_body_impl(
) noexcept {
  if (this->body) {
    return this->body.get();
  }

  try {
    this->body = std::make_unique<json>(json::parse(this->req->body));
    return this->body.get();
  } catch (std::exception& err) {
    return tl::unexpected<entity::Log>(
        {.msg = err.what(),
         .file = "adapters/http/wrapper.cpp",
         .function = "WrapperRequest::get_body_impl()"}
    );
  }
}

template <>
[[nodiscard]] tl::expected<fb::LoginRequest*, entity::Log>
WrapperRequest<fb::LoginRequest, interface::DefStore, interface::DevLogger>::
    get_body_impl() noexcept {
  try {
    return flatbuffers::GetMutableRoot<fb::LoginRequest>(this->req->body.data());
  } catch (std::exception& err) {
    return tl::unexpected<entity::Log>(
        {.msg = err.what(),
         .file = "adapters/http/wrapper.cpp",
         .function = "WrapperRequest::get_body_impl()"}
    );
  }
}

template <>
[[nodiscard]] tl::expected<std::nullopt_t*, entity::Log>
WrapperRequest<std::nullopt_t, interface::DefStore, interface::DefLogger>::
    get_body_impl() noexcept {
  return nullptr;
}

// Response
void WrapperResponse::set_content_type_impl(const ContentType& type) noexcept {
  switch (type) {
  case ContentType::application_json:
    this->res->content_type =
        http::server::response::content_type_enum::application_json;
    break;
  case ContentType::text_html:
    this->res->content_type =
        http::server::response::content_type_enum::text_html;
    break;
  }
}

template <>
void WrapperResponse::set_body_impl(const std::string& body) noexcept {
  this->res->content = body;
  this->res->content_length = std::to_string(body.length());
}

void WrapperResponse::set_status_impl(const ResponseStatus& status) noexcept {
  this->res->status = static_cast<int>(status);
}

} // namespace adapter
