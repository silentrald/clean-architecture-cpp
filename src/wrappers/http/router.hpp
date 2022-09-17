#ifndef HTTP_ROUTER_HPP
#define HTTP_ROUTER_HPP

#include "request.hpp"
#include "response.hpp"
#include <memory>
#include <string>
#include <unordered_map>

namespace http::server {

struct router {
  std::string method;
  std::string path;
  void (*cb)(request* req, response* res);
};

struct router_node {
  std::unordered_map<std::string, std::unique_ptr<router_node>> map;
  std::unique_ptr<router_node> dynamic_ptr;
  void (*cb)(request* req, response* res);
};

} // namespace http::server

#endif

