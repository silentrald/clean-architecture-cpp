//
// server.hpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2022 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef HTTP_SERVER_HPP
#define HTTP_SERVER_HPP

#include "connection.hpp"
#include "connection_manager.hpp"
#include "request_handler.hpp"
#include <asio.hpp>
#include <string>
#include <thread>
#include <vector>

namespace http::server {

class server {
private:
  asio::io_context ctx;
  asio::signal_set signals;
  asio::ip::tcp::acceptor acceptor;
  connection_manager cm;
  request_handler handler;

  // For multihreading
  uint num_threads;
  std::vector<std::thread> threads;

  void do_accept();
  void do_await_stop();

public:
  server() = delete;
  server(const server&) = delete;
  server(server&&) = delete;
  server& operator=(const server&) = delete;
  server& operator=(server&&) = delete;
  ~server() = default;

  void run();
  explicit server(
      const std::string& address, const std::string& port,
      const std::vector<router>& routers, const std::string& base_path = "/",
      uint num_threads = 1U
  );
};

} // namespace http::server

#endif // HTTP_SERVER_HPP
