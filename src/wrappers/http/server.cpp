//
// server.cpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2022 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include "server.hpp"
#include <csignal>
#include <utility>

namespace http::server {

server::server(
    const std::string& address, const std::string& port,
    const std::vector<router>& routers, const std::string& base_path,
    uint num_threads
)
    : ctx(1), signals(this->ctx), acceptor(this->ctx), num_threads(num_threads),
      handler(request_handler(routers, base_path)) {
  this->signals.add(SIGINT);
  this->signals.add(SIGTERM);
#if defined(SIGQUIT)
  this->signals.add(SIGQUIT);
#endif // defined(SIGQUIT)

  if (this->num_threads > std::thread::hardware_concurrency()) {
    this->num_threads = std::thread::hardware_concurrency();
  }

  this->do_await_stop();

  asio::ip::tcp::resolver resolver(this->ctx);
  asio::ip::tcp::endpoint endpoint = *resolver.resolve(address, port).begin();
  acceptor.open(endpoint.protocol());
  acceptor.set_option(asio::ip::tcp::acceptor::reuse_address(true));
  acceptor.bind(endpoint);
  acceptor.listen();

  this->do_accept();
}

void server::run() {
  if (this->num_threads == 1) {
    this->ctx.run();
  } else {
    // Create a thread pool
    for (uint i = 0U; i < this->num_threads; i++) {
      this->threads.emplace_back([this] { this->ctx.run(); });
    }

    for (auto& thread : this->threads) {
      if (thread.joinable()) {
        thread.join();
      }
    }
  }
}

void server::do_accept() {
  this->acceptor.async_accept(
      [this](std::error_code ec, asio::ip::tcp::socket socket) {
        // Check whether the server was stopped by a signal before this
        // completion handler had a chance to run.
        if (!this->acceptor.is_open()) {
          return;
        }

        if (!ec) {
          this->cm.start(std::make_shared<connection>(
              std::move(socket), this->cm, this->handler
          ));
        }

        this->do_accept();
      }
  );
}

void server::do_await_stop() {
  this->signals.async_wait([this](std::error_code /*ec*/, int /*signo*/) {
    this->acceptor.close();
    this->cm.stop_all();
  });
}

} // namespace http::server

