#include "./mock.hpp"
#include "adapters/http/build.hpp"

namespace adapter {

void MockResponse::set_content_type_impl(const ContentType& type) noexcept {
  this->content_type = type;
}

template <> void MockResponse::set_body_impl(const std::string& body) noexcept {
  this->body = body;
}

void MockResponse::set_status_impl(const ResponseStatus& status) noexcept {
  this->status = status;
}

} // namespace adapter

