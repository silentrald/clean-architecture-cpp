#include "./wrapper.hpp"
#include "fbs/auth.hpp"
#include "interfaces/logger/dev.hpp"
#include "interfaces/logger/singleton.hpp"
#include "interfaces/store/singleton.hpp"
#include "tl/expected.hpp"
#include <cstdint>
#include <cstring>
#include <exception>
#include <flatbuffers/buffer.h>
#include <flatbuffers/table.h>
#include <flatbuffers/verifier.h>
#include <memory>
#include <optional>
#include <string>

namespace adapter {
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
  this->res->content_length = std::to_string(body.size());
}

template <>
void WrapperResponse::set_body_impl(const std::vector<std::string>& body) noexcept {
  this->res->content = '[';
  for (const std::string& row : body) {
    this->res->content.append('"' + row + "\",");
  }
  this->res->content.back() = ']';
  this->res->content_length = std::to_string(this->res->content.size());
}

void WrapperResponse::set_body_impl(const char* body) noexcept {
  this->res->content = body;
  this->res->content_length = std::to_string(this->res->content.size());
}

void WrapperResponse::set_status_impl(const ResponseStatus& status) noexcept {
  this->res->status = static_cast<int>(status);
}

} // namespace adapter
