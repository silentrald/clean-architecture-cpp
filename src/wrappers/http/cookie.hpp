#ifndef HTTP_COOKIE_HTTP
#define HTTP_COOKIE_HTTP

#include <string>

namespace http::server {
struct cookie {
  std::string name;
  std::string value;
  std::string path;
  std::string domain;
  std::string same_site;
  int max_age = -1;
  bool secure;
  bool http_only;
  std::string cookie_value;

  [[nodiscard]] std::string& to_string() {
    if (!this->cookie_value.empty()) {
      return this->cookie_value;
    }

    if (this->name.empty()) {
      this->cookie_value = "";
      return this->cookie_value;
    }

    std::string val = this->name + "=" + this->value;
    if (!this->path.empty()) {
      val.append("; Path=" + this->path);
    }

    if (!this->domain.empty()) {
      val.append("; Domain=" + this->domain);
    }

    if (!this->same_site.empty()) {
      val.append("; SameSite=" + this->same_site);
    }

    if (this->max_age > -1) {
      val.append("; MaxAge=" + std::to_string(this->max_age));
    }

    if (this->secure) {
      val.append("; Secure");
    }

    if (this->http_only) {
      val.append("; HttpOnly");
    }

    this->cookie_value = val;
    return this->cookie_value;
  }
};
} // namespace http::server

#endif

