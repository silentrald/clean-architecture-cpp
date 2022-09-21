//
// response.hpp
// ~~~~~~~~~
//
// Copyright (c) 2003-2022 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef HTTP_REPLY_HPP
#define HTTP_REPLY_HPP

#include "cookie.hpp"
#include "header.hpp"
#include <asio.hpp>
#include <string>
#include <vector>

namespace http::server {

const char* const ContentTypeStrings[] = {"application/json"};

const int STATUS_OK = 200;
const int STATUS_CREATED = 201;
const int STATUS_ACCEPTED = 202;
const int STATUS_NO_CONTENT = 204;
const int STATUS_MULTIPLE_CHOICES = 300;
const int STATUS_MOVED_PERMANENTLY = 301;
const int STATUS_MOVED_TEMPORARILY = 302;
const int STATUS_NOT_MODIFIED = 303;
const int STATUS_BAD_REQUEST = 400;
const int STATUS_UNAUTHORIZED = 401;
const int STATUS_FORBIDDEN = 403;
const int STATUS_NOT_FOUND = 404;
const int STATUS_INTERNAL_SERVER_ERROR = 500;
const int STATUS_NOT_IMPLEMENTED = 501;
const int STATUS_BAD_GATEWAY = 502;
const int STATUS_SERVICE_UNAVAILABLE = 503;

struct response {
  int status = 200; // NOLINT
  std::vector<header> headers;
  std::vector<cookie> cookies;
  std::string content{};
  std::string content_length = "0";

  enum content_type_enum {
    application_json = 0,
    text_html = 1
  } content_type = content_type_enum::application_json;

  std::vector<asio::const_buffer> to_buffers();
  static response stock_response(int status);
};

} // namespace http::server

#endif // HTTP_REPLY_HPP
