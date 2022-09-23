//
// connection.cpp
// ~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2022 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include "connection.hpp"
#include "asio/buffer.hpp"
#include "asio/error.hpp"
#include "asio/error_code.hpp"
#include "asio/impl/read_until.hpp"
#include "asio/read.hpp"
#include "asio/read_until.hpp"
#include "asio/socket_base.hpp"
#include "asio/write.hpp"
#include "connection_manager.hpp"
#include "request_handler.hpp"
#include "wrappers/http/cookie.hpp"
#include "wrappers/http/request.hpp"
#include "wrappers/http/response.hpp"
#include <cstddef>
#include <iostream>
#include <system_error>
#include <utility>
#include <vector>

namespace http::server {
const int BASE = 10;

connection::connection(
    asio::ip::tcp::socket socket, connection_manager& manager,
    request_handler& handler
)
    : socket(std::move(socket)),
      connection_manager_(manager),
      request_handler_(handler),
      buffer() {}

void connection::start() {
  this->do_read();
}

void connection::stop() {
  socket.close();
}

void connection::parse_cookie(const std::string& cookie_string) {
  size_t start_cursor = 0U;
  size_t mid_cursor = 0U;
  size_t end_cursor = 0U;
  std::string tmp{};
  std::string key{};

  while ((end_cursor = cookie_string.find(';', start_cursor)) !=
         std::string::npos) {
    tmp = cookie_string.substr(start_cursor, end_cursor - start_cursor);
    mid_cursor = tmp.find('=');
    if (mid_cursor != std::string::npos) {
      key = tmp.substr(0, mid_cursor);
      if (key == "SESSION") { // TODO: Make this configurable
        req.session_id = tmp.substr(mid_cursor + 1);
      } else {
        req.cookies[key] = tmp.substr(mid_cursor + 1);
      }
    }
    start_cursor = end_cursor + 2;
  }

  tmp = cookie_string.substr(start_cursor);
  mid_cursor = tmp.find('=');
  if (mid_cursor != std::string::npos) {
    key = tmp.substr(0, mid_cursor);
    if (key == "SESSION") {
      req.session_id = tmp.substr(mid_cursor + 1);
    } else {
      req.cookies[key] = tmp.substr(mid_cursor + 1);
    }
  }
}

void connection::handle_header_buffer(std::size_t bytes_transferred) {
  request_parser::result_type result{};
  std::array<char, BUFFER_SIZE>::iterator it{};

  std::tie(result, it) = request_parser_.parse(
      this->req, this->buffer.data(), this->buffer.data() + bytes_transferred
  );

  if (result == request_parser::good) {
    for (auto& h : this->req.headers) {
      if (h.name == "Content-Length") {
        this->req.content_len = std::stoi(h.value);
      } else if (h.name == "Content-Type") {
        this->req.content_type = h.value;
      } else if (h.name == "Cookie") {
        this->parse_cookie(h.value);
      }
    }

    // Keep Alive

    // Get the remaining content
    uint body_size = bytes_transferred + (this->buffer.data() - it);
    if (this->socket.available() == 0) {
      if (body_size > this->req.content_len) {
        this->res = response::stock_response(STATUS_BAD_REQUEST);
        this->do_write();
      } else {
        this->req.body = std::string{it, body_size};
        this->request_handler_.handle_request(&this->req, &this->res);
        this->do_write();
      }
    } else {
      this->req.body = std::string{it, body_size};
      this->header_buffer = false;
    }
  } else if (result == request_parser::bad) {
    this->res = response::stock_response(STATUS_BAD_REQUEST);
    this->do_write();
  } else { // Indeterminate
    // Still read the header value
    this->do_read();
  }
}

void connection::handle_body_buffer(std::size_t bytes_transferred) {
  this->req.body.append(std::string{this->buffer.begin(), bytes_transferred});

  if (this->req.body.size() > this->req.content_len) {
    this->header_buffer = true;
    this->res = response::stock_response(STATUS_BAD_REQUEST);
    this->do_write();
  } else if (this->socket.available() == 0) {
    this->header_buffer = true;
    this->request_handler_.handle_request(&this->req, &this->res);
    this->do_write();
  } else {
    this->do_read();
  }
}

void connection::do_read() {
  /* auto self(shared_from_this()); */

  /* asio::async_read_until( */
  /* this->socket, this->stream_buff, "\r\n\r\n", */
  this->socket.async_read_some(
      asio::buffer(this->buffer),
      [this](std::error_code ec, std::size_t bytes_transferred) {
        if (!ec) {
          /* this->res.set_content("Hello World"); */
          /* this->do_write(); */

          if (this->header_buffer) {
            this->handle_header_buffer(bytes_transferred);
          } else {
            this->handle_body_buffer(bytes_transferred);
          }
          return;
        }

        /* if (ec == asio::error::eof) { */
        // Add a timer here
        /* std::cout << "Rereading\n"; */
        /* this->do_read(); */
        /* return; */
        /* } */

        if (ec != asio::error::operation_aborted) {
          std::error_code ignored_ec;
          this->socket.shutdown(
              asio::ip::tcp::socket::shutdown_both, ignored_ec
          );
          this->connection_manager_.stop(shared_from_this());
        }
      }
  );
}

void connection::do_write() {
  /* auto self(shared_from_this()); */

  asio::async_write(
      this->socket, this->res.to_buffers(),
      [this](std::error_code ec, std::size_t) {
        if (ec == asio::error::operation_aborted) {
          return;
        }

        this->do_read();
      }
  );
}

} // namespace http::server

