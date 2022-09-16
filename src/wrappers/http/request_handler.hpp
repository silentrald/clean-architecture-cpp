//
// request_handler.hpp
// ~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2022 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef HTTP_REQUEST_HANDLER_HPP
#define HTTP_REQUEST_HANDLER_HPP

#include "router.hpp"
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace http::server {

struct response;
struct request;

class request_handler {
private:
  std::string base_path;
  // Key: http method
  std::unordered_map<std::string, std::unique_ptr<router_node>> map;
  static bool url_decode(const std::string& in, std::string& out);

public:
  request_handler() = delete;
  request_handler(const request_handler&) = delete;
  request_handler(request_handler&&) = delete;
  request_handler& operator=(const request_handler&) = delete;
  request_handler& operator=(request_handler&&) = delete;
  ~request_handler() = default;

  explicit request_handler(const std::vector<router>& routers, const std::string& base_path = "/");

  void handle_request(request* req, response* res);
};

} // namespace http::server

#endif // HTTP_REQUEST_HANDLER_HPP
