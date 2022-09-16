//
// request.hpp
// ~~~~~~~~~~~
//
// Copyright (c) 2003-2022 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef HTTP_REQUEST_HPP
#define HTTP_REQUEST_HPP

#include "header.hpp"
#include "cookie.hpp"
#include <string>
#include <unordered_map>
#include <vector>

namespace http::server {

struct request {
  std::string method;
  std::string uri;
  int http_version_major;
  int http_version_minor;
  uint content_len;
  std::string body;
  std::string session_id;
  std::vector<header> headers;
  std::unordered_map<std::string, std::string> cookies;
};

} // namespace http::server

#endif // HTTP_REQUEST_HPP
