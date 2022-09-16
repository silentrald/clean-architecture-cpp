#ifndef LOGGER_INTERFACE_DEV_HPP
#define LOGGER_INTERFACE_DEV_HPP

#include "./build.hpp"
#include <array>
#include <cstdio>
#include <mutex>
#include <string>
#include <sys/time.h>

namespace interface {
const int DATE_TIME_LENGTH = sizeof "9999-12-31T23:59:59.999";

class DevLogger : public ILogger<DevLogger> {
private:
  enum Logging level;
  std::mutex logger_mutex;
  std::array<char, DATE_TIME_LENGTH> date_time{};

  std::string now();

public:
  explicit DevLogger(enum Logging level);

  void fatal_impl(const char* text);
  void error_impl(const char* text);
  void warn_impl(const char* text);
  void info_impl(const char* text);
  void debug_impl(const char* text);
  void trace_impl(const char* text);
};
} // namespace interface

#endif
