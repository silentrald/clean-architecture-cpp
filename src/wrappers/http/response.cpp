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
#include <vector>

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

asio::const_buffer to_buffer(int status) {
  switch (status) {
  case STATUS_OK:
    return asio::buffer(ok);
  case STATUS_CREATED:
    return asio::buffer(created);
  case STATUS_ACCEPTED:
    return asio::buffer(accepted);
  case STATUS_NO_CONTENT:
    return asio::buffer(no_content);
  case STATUS_MULTIPLE_CHOICES:
    return asio::buffer(multiple_choices);
  case STATUS_MOVED_PERMANENTLY:
    return asio::buffer(moved_permanently);
  case STATUS_MOVED_TEMPORARILY:
    return asio::buffer(moved_temporarily);
  case STATUS_NOT_MODIFIED:
    return asio::buffer(not_modified);
  case STATUS_BAD_REQUEST:
    return asio::buffer(bad_request);
  case STATUS_UNAUTHORIZED:
    return asio::buffer(unauthorized);
  case STATUS_FORBIDDEN:
    return asio::buffer(forbidden);
  case STATUS_NOT_FOUND:
    return asio::buffer(not_found);
  case STATUS_INTERNAL_SERVER_ERROR:
    return asio::buffer(internal_server_error);
  case STATUS_NOT_IMPLEMENTED:
    return asio::buffer(not_implemented);
  case STATUS_BAD_GATEWAY:
    return asio::buffer(bad_gateway);
  case STATUS_SERVICE_UNAVAILABLE:
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

const std::vector<std::string> content_type_strings = {
    "application/json", "text/html"};

} // namespace misc_strings

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

  // Set the Content Type and Content Lengtif (this->status != 204) {
  if (this->status != STATUS_NO_CONTENT) {
    buffers.emplace_back(asio::buffer(misc_strings::content_type));
    buffers.emplace_back(
        asio::buffer(misc_strings::content_type_strings[this->content_type])
    );
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

std::string to_string(int status) {
  switch (status) {
  case STATUS_OK:
    return ok;
  case STATUS_CREATED:
    return created;
  case STATUS_ACCEPTED:
    return accepted;
  case STATUS_NO_CONTENT:
    return no_content;
  case STATUS_MULTIPLE_CHOICES:
    return multiple_choices;
  case STATUS_MOVED_PERMANENTLY:
    return moved_permanently;
  case STATUS_MOVED_TEMPORARILY:
    return moved_temporarily;
  case STATUS_NOT_MODIFIED:
    return not_modified;
  case STATUS_BAD_REQUEST:
    return bad_request;
  case STATUS_UNAUTHORIZED:
    return unauthorized;
  case STATUS_FORBIDDEN:
    return forbidden;
  case STATUS_NOT_FOUND:
    return not_found;
  case STATUS_INTERNAL_SERVER_ERROR:
    return internal_server_error;
  case STATUS_NOT_IMPLEMENTED:
    return not_implemented;
  case STATUS_BAD_GATEWAY:
    return bad_gateway;
  case STATUS_SERVICE_UNAVAILABLE:
    return service_unavailable;
  default:
    return internal_server_error;
  }
}

} // namespace stock_responses

response response::stock_response(int status) {
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

