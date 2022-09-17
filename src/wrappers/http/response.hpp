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

struct response {
  enum status_type {
    ok = 200,
    created = 201,
    accepted = 202,
    no_content = 204,
    multiple_choices = 300,
    moved_permanently = 301,
    moved_temporarily = 302,
    not_modified = 304,
    bad_request = 400,
    unauthorized = 401,
    forbidden = 403,
    not_found = 404,
    internal_server_error = 500,
    not_implemented = 501,
    bad_gateway = 502,
    service_unavailable = 503
  } status = status_type::ok;

  std::vector<header> headers;
  std::vector<cookie> cookies;
  std::string content{};
  std::string content_length = "0";

  /* enum content_type_enum { */
  /*   text_html = 0, */
  /* } content_type = content_type_enum::text_html; */

  void set_content(const std::string& content);
  void set_content(const char* content);
  std::vector<asio::const_buffer> to_buffers();
  static response stock_response(status_type status);
};

} // namespace http::server

#endif // HTTP_REPLY_HPP
