//
// connection.hpp
// ~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2022 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef HTTP_CONNECTION_HPP
#define HTTP_CONNECTION_HPP

#include "request.hpp"
#include "request_handler.hpp"
#include "request_parser.hpp"
#include "response.hpp"
#include <array>
#include <asio.hpp>
#include <cstddef>
#include <memory>

namespace http::server {

class connection_manager;

const uint BUFFER_SIZE = 8 * 1024; // 8KB

/// Represents a single connection from a client.
class connection : public std::enable_shared_from_this<connection> {
private:
  asio::ip::tcp::socket socket;
  connection_manager& connection_manager_;
  request_handler& request_handler_;

  // NOTE: The first buffer which contains the headers
  //  that limits the header to have the value of BUFFER_SIZE
  std::array<char, BUFFER_SIZE> buffer;
  bool header_buffer = true;

  request req;
  request_parser request_parser_;
  response res;

  void do_read();
  void do_write();

  void parse_cookie(const std::string& cookie_string);
  void handle_header_buffer(std::size_t bytes_transferred);
  void handle_body_buffer(std::size_t bytes_transferred);

public:
  connection() = delete;
  connection(const connection&) = delete;
  connection(connection&&) = delete;
  connection& operator=(const connection&) = delete;
  connection& operator=(connection&&) = delete;
  ~connection() = default;

  explicit connection(
      asio::ip::tcp::socket socket, connection_manager& manager,
      request_handler& handler
  );

  void start();
  void stop();
};

using connection_ptr = std::shared_ptr<connection>;

} // namespace http::server

#endif // HTTP_CONNECTION_HPP
