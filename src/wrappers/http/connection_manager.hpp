//
// connection_manager.hpp
// ~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2022 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef HTTP_CONNECTION_MANAGER_HPP
#define HTTP_CONNECTION_MANAGER_HPP

#include "connection.hpp"
#include <unordered_set>

namespace http::server {

class connection_manager {
private:
  std::unordered_set<connection_ptr> connections;

public:
  connection_manager() = default;
  connection_manager(const connection_manager&) = delete;
  connection_manager(connection_manager&&) = delete;
  connection_manager& operator=(const connection_manager&) = delete;
  connection_manager& operator=(connection_manager&&) = delete;
  ~connection_manager() = default;

  void start(const connection_ptr& c);
  void stop(const connection_ptr& c);
  void stop_all();
};

} // namespace http::server

#endif // HTTP_CONNECTION_MANAGER_HPP
