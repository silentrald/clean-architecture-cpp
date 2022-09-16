//
// connection_manager.cpp
// ~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2022 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include "connection_manager.hpp"

namespace http::server {

void connection_manager::start(const connection_ptr& c) {
  this->connections.insert(c);
  c->start();
}

void connection_manager::stop(const connection_ptr& c) {
  this->connections.erase(c);
  c->stop();
}

void connection_manager::stop_all() {
  for (const auto& c : this->connections) {
    c->stop();
  }

  this->connections.clear();
}

} // namespace http::server

