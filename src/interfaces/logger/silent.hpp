#ifndef LOGGER_INTERFACE_SILENT_HPP
#define LOGGER_INTERFACE_SILENT_HPP

#include "./build.hpp"

namespace interface {
class SilentLogger : ILogger<SilentLogger> {
public:
  void fatal_impl(const char* text) {}
  void error_impl(const char* text) {}
  void warn_impl(const char* text) {}
  void info_impl(const char* text) {}
  void debug_impl(const char* text) {}
  void trace_impl(const char* text) {}
};

} // namespace interface

#endif

