#include "./wrapper.hpp"
#include <cstring>
#include <exception>

namespace adapter {

template <>
[[nodiscard]] tl::expected<json, entity::Log>
WrapperRequest<json, interface::DefStore, interface::DefLogger>::get_body_impl() noexcept {
  try {
    return json::parse(this->req->body);
  } catch (std::exception& err) {
    return tl::unexpected<entity::Log>(
        {.msg = err.what(),
         .file = "adapters/http/wrapper.cpp",
         .function = "WrapperRequest::get_body()"}
    );
  }
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
