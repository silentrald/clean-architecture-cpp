//
// request_handler.cpp
// ~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2022 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include "request_handler.hpp"
#include "mime_types.hpp"
#include "request.hpp"
#include "response.hpp"
#include <cstddef>
#include <iostream>
#include <memory>

namespace http::server {

request_handler::request_handler(
    const std::vector<router>& routers, const std::string& base_path
)
    : base_path(base_path) {
  size_t start_cursor = 0U;
  size_t end_cursor = 0U;
  router_node* ptr = nullptr;
  std::string path{};

  for (const auto& rtr : routers) {
    // Get parent node
    /* rtr.method = rtr.method.toupper(); */
    if (this->map.find(rtr.method) == this->map.end()) {
      this->map[rtr.method] = std::make_unique<router_node>();
    }
    ptr = this->map[rtr.method].get();

    if (rtr.path == "/") {
      ptr->cb = rtr.cb;
      continue;
    }

    start_cursor = 1U;
    // Loop here // NOTE: No dynamic yet
    while ((end_cursor = rtr.path.find('/', start_cursor)) != std::string::npos
    ) {
      path = rtr.path.substr(start_cursor, end_cursor - start_cursor);
      start_cursor = end_cursor + 1;

      if (ptr->map.find(path) == ptr->map.end()) {
        ptr->map[path] = std::make_unique<router_node>();
      }
      ptr = ptr->map[path].get();
    }

    // Leaf node
    path = rtr.path.substr(start_cursor);
    if (ptr->map.find(path) == ptr->map.end()) {
      ptr->map[path] = std::make_unique<router_node>();
    }
    ptr->map[path]->cb = rtr.cb;
  }
}

void request_handler::handle_request(request* req, response* res) {
  /* std::string request_path; */
  /* if (!url_decode(req.uri, request_path)) { */
  /*   res = response::stock_response(response::bad_request); */
  /*   return; */
  /* } */

  // NOTE: Can use middlewares here

  std::string path = req->uri.substr(0, req->uri.find('?'));
  if (!path.starts_with(this->base_path)) {
    res->status = response::not_found;
    return;
  }

  auto pos = this->map.find(req->method);
  if (pos == this->map.end()) {
    res->status = response::not_found;
    return;
  }

  router_node* node = pos->second.get();
  // Base Case: only "/" path
  if (path.size() == this->base_path.size()) {
    if (node->cb == nullptr) {
      res->status = response::not_found;
    } else {
      node->cb(req, res);
    }
    return;
  }

  size_t start_cursor = this->base_path.size() + 1;
  size_t end_cursor = 0U;

  while ((end_cursor = path.find('/', start_cursor)) != std::string::npos) {
    pos = node->map.find(path.substr(start_cursor, end_cursor - start_cursor));
    if (pos == node->map.end()) {
      res->status = response::not_found;
      return;
    }
    node = pos->second.get();
    start_cursor = end_cursor + 1;
  }

  // Check if last word if the path
  pos = node->map.find(path.substr(start_cursor));
  if (pos == node->map.end()) {
    res->status = response::not_found;
    return;
  }
  node = pos->second.get();

  if (node->cb == nullptr) {
    res->status = response::not_found;
    return;
  }

  node->cb(req, res);
}

bool request_handler::url_decode(const std::string& in, std::string& out) {
  out.clear();
  out.reserve(in.size());
  for (std::size_t i = 0; i < in.size(); ++i) {
    if (in[i] == '%') {
      if (i + 3 <= in.size()) {
        int value = 0;
        std::istringstream is(in.substr(i + 1, 2));
        if (is >> std::hex >> value) {
          out += static_cast<char>(value);
          i += 2;
        } else {
          return false;
        }
      } else {
        return false;
      }
    } else if (in[i] == '+') {
      out += ' ';
    } else {
      out += in[i];
    }
  }
  return true;
}

} // namespace http::server

