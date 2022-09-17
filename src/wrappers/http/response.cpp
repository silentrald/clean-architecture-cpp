//
// response.cpp
// ~~~~~~~~~
//
// Copyright (c) 2003-2022 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include "response.hpp"
#include "asio/buffer.hpp"
#include "wrappers/http/header.hpp"
#include <cstddef>
#include <iostream>
#include <memory>
#include <string>
#include <sys/types.h>

namespace http::server {

namespace status_strings {

const std::string ok = "HTTP/1.1 200 OK\r\n";
const std::string created = "HTTP/1.1 201 Created\r\n";
const std::string accepted = "HTTP/1.1 202 Accepted\r\n";
const std::string no_content = "HTTP/1.1 204 No Content\r\n";
const std::string multiple_choices = "HTTP/1.1 300 Multiple Choices\r\n";
const std::string moved_permanently = "HTTP/1.1 301 Moved Permanently\r\n";
const std::string moved_temporarily = "HTTP/1.1 302 Moved Temporarily\r\n";
const std::string not_modified = "HTTP/1.1 304 Not Modified\r\n";
const std::string bad_request = "HTTP/1.1 400 Bad Request\r\n";
const std::string unauthorized = "HTTP/1.1 401 Unauthorized\r\n";
const std::string forbidden = "HTTP/1.1 403 Forbidden\r\n";
const std::string not_found = "HTTP/1.1 404 Not Found\r\n";
const std::string internal_server_error =
    "HTTP/1.1 500 Internal Server Error\r\n";
const std::string not_implemented = "HTTP/1.1 501 Not Implemented\r\n";
const std::string bad_gateway = "HTTP/1.1 502 Bad Gateway\r\n";
const std::string service_unavailable = "HTTP/1.1 503 Service Unavailable\r\n";

asio::const_buffer to_buffer(response::status_type status) {
  switch (status) {
  case response::ok:
    return asio::buffer(ok);
  case response::created:
    return asio::buffer(created);
  case response::accepted:
    return asio::buffer(accepted);
  case response::no_content:
    return asio::buffer(no_content);
  case response::multiple_choices:
    return asio::buffer(multiple_choices);
  case response::moved_permanently:
    return asio::buffer(moved_permanently);
  case response::moved_temporarily:
    return asio::buffer(moved_temporarily);
  case response::not_modified:
    return asio::buffer(not_modified);
  case response::bad_request:
    return asio::buffer(bad_request);
  case response::unauthorized:
    return asio::buffer(unauthorized);
  case response::forbidden:
    return asio::buffer(forbidden);
  case response::not_found:
    return asio::buffer(not_found);
  case response::internal_server_error:
    return asio::buffer(internal_server_error);
  case response::not_implemented:
    return asio::buffer(not_implemented);
  case response::bad_gateway:
    return asio::buffer(bad_gateway);
  case response::service_unavailable:
    return asio::buffer(service_unavailable);
  default:
    return asio::buffer(internal_server_error);
  }
}

} // namespace status_strings

namespace misc_strings {

const char name_value_separator[] = {':', ' '};
const char crlf[] = {'\r', '\n'};
const char content_type[] = {'C', 'o', 'n', 't', 'e', 'n', 't',
                             '-', 'T', 'y', 'p', 'e', ':', ' '};
const char content_length[] = {'C', 'o', 'n', 't', 'e', 'n', 't', '-',
                               'L', 'e', 'n', 'g', 't', 'h', ':', ' '};
const char set_cookie[] = {'S', 'e', 't', '-', 'C', 'o',
                           'o', 'k', 'i', 'e', ':', ' '};
const char text_html[] = {'t', 'e', 'x', 't', '/', 'h', 't', 'm', 'l'};

} // namespace misc_strings

void response::set_content(const std::string& content) {
  this->content = content;
  this->content_length = std::to_string(content.size());
}

void response::set_content(const char* content) {
  this->content = content;
  this->content_length = std::to_string(this->content.size());
}

std::vector<asio::const_buffer> response::to_buffers() {
  std::vector<asio::const_buffer> buffers;
  buffers.push_back(status_strings::to_buffer(this->status));

  for (auto& h : this->headers) {
    buffers.emplace_back(asio::buffer(h.name));
    buffers.emplace_back(asio::buffer(misc_strings::name_value_separator));
    buffers.emplace_back(asio::buffer(h.value));
    buffers.emplace_back(asio::buffer(misc_strings::crlf));
  }

  for (auto& c : this->cookies) {
    buffers.emplace_back(asio::buffer(misc_strings::set_cookie));
    buffers.emplace_back(asio::buffer(c.to_string()));
    buffers.emplace_back(asio::buffer(misc_strings::crlf));
  }

  // Set the Content Type and Content Length
  if (this->status != response::no_content) {
    buffers.emplace_back(asio::buffer(misc_strings::content_type));
    buffers.emplace_back(asio::buffer(misc_strings::text_html));
    buffers.emplace_back(asio::buffer(misc_strings::crlf));

    buffers.emplace_back(asio::buffer(misc_strings::content_length));
    buffers.emplace_back(asio::buffer(this->content_length));
    buffers.emplace_back(asio::buffer(misc_strings::crlf));
  }

  buffers.emplace_back(asio::buffer(misc_strings::crlf));
  buffers.emplace_back(asio::buffer(content));

  return buffers;
}

namespace stock_responses {

const char* const ok = "";
const char* const created = "<html>"
                            "<head><title>Created</title></head>"
                            "<body><h1>201 Created</h1></body>"
                            "</html>";
const char* const accepted = "<html>"
                             "<head><title>Accepted</title></head>"
                             "<body><h1>202 Accepted</h1></body>"
                             "</html>";
const char* const no_content = "<html>"
                               "<head><title>No Content</title></head>"
                               "<body><h1>204 Content</h1></body>"
                               "</html>";
const char* const multiple_choices =
    "<html>"
    "<head><title>Multiple Choices</title></head>"
    "<body><h1>300 Multiple Choices</h1></body>"
    "</html>";
const char* const moved_permanently =
    "<html>"
    "<head><title>Moved Permanently</title></head>"
    "<body><h1>301 Moved Permanently</h1></body>"
    "</html>";
const char* const moved_temporarily =
    "<html>"
    "<head><title>Moved Temporarily</title></head>"
    "<body><h1>302 Moved Temporarily</h1></body>"
    "</html>";
const char* const not_modified = "<html>"
                                 "<head><title>Not Modified</title></head>"
                                 "<body><h1>304 Not Modified</h1></body>"
                                 "</html>";
const char* const bad_request = "<html>"
                                "<head><title>Bad Request</title></head>"
                                "<body><h1>400 Bad Request</h1></body>"
                                "</html>";
const char* const unauthorized = "<html>"
                                 "<head><title>Unauthorized</title></head>"
                                 "<body><h1>401 Unauthorized</h1></body>"
                                 "</html>";
const char* const forbidden = "<html>"
                              "<head><title>Forbidden</title></head>"
                              "<body><h1>403 Forbidden</h1></body>"
                              "</html>";
const char* const not_found = "<html>"
                              "<head><title>Not Found</title></head>"
                              "<body><h1>404 Not Found</h1></body>"
                              "</html>";
const char* const internal_server_error =
    "<html>"
    "<head><title>Internal Server Error</title></head>"
    "<body><h1>500 Internal Server Error</h1></body>"
    "</html>";
const char* const not_implemented =
    "<html>"
    "<head><title>Not Implemented</title></head>"
    "<body><h1>501 Not Implemented</h1></body>"
    "</html>";
const char* const bad_gateway = "<html>"
                                "<head><title>Bad Gateway</title></head>"
                                "<body><h1>502 Bad Gateway</h1></body>"
                                "</html>";
const char* const service_unavailable =
    "<html>"
    "<head><title>Service Unavailable</title></head>"
    "<body><h1>503 Service Unavailable</h1></body>"
    "</html>";

std::string to_string(response::status_type status) {
  switch (status) {
  case response::ok:
    return ok;
  case response::created:
    return created;
  case response::accepted:
    return accepted;
  case response::no_content:
    return no_content;
  case response::multiple_choices:
    return multiple_choices;
  case response::moved_permanently:
    return moved_permanently;
  case response::moved_temporarily:
    return moved_temporarily;
  case response::not_modified:
    return not_modified;
  case response::bad_request:
    return bad_request;
  case response::unauthorized:
    return unauthorized;
  case response::forbidden:
    return forbidden;
  case response::not_found:
    return not_found;
  case response::internal_server_error:
    return internal_server_error;
  case response::not_implemented:
    return not_implemented;
  case response::bad_gateway:
    return bad_gateway;
  case response::service_unavailable:
    return service_unavailable;
  default:
    return internal_server_error;
  }
}

} // namespace stock_responses

response response::stock_response(response::status_type status) {
  response res;
  res.status = status;
  res.content = stock_responses::to_string(status);
  res.headers.resize(2);
  res.headers[0].name = "Content-Length";
  res.headers[0].value = std::to_string(res.content.size());
  res.headers[1].name = "Content-Type";
  res.headers[1].value = "text/html";
  return res;
}

} // namespace http::server

