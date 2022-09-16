#ifndef LOGGER_INTERFACE_BUILD_HPP
#define LOGGER_INTERFACE_BUILD_HPP

#include <execinfo.h>
#include <string>

namespace interface {
enum Logging {
  LOGGING_SILENT = 0,
  LOGGING_FATAL = 1,
  LOGGING_ERROR = 2,
  LOGGING_WARN = 3,
  LOGGING_INFO = 4,
  LOGGING_DEBUG = 5,
  LOGGING_TRACE = 6
};

template <typename T> class ILogger {
private:
  ILogger() = default;

public:
  void fatal(const std::string& text) {
    static_cast<T*>(this)->fatal_impl(text.c_str());
  }

  void fatal(const char* text) {
    static_cast<T*>(this)->fatal_impl(text);
  }

  void error(const std::string& text) {
    static_cast<T*>(this)->error_impl(text.c_str());
  }

  void error(const char* text) {
    static_cast<T*>(this)->error_impl(text);
  }

  void warn(const std::string& text) {
    static_cast<T*>(this)->warn_impl(text.c_str());
  }

  void warn(const char* text) {
    static_cast<T*>(this)->warn_impl(text);
  }

  void info(const std::string& text) {
    static_cast<T*>(this)->info_impl(text.c_str());
  }

  void info(const char* text) {
    static_cast<T*>(this)->info_impl(text);
  }

  void debug(const std::string& text) {
    static_cast<T*>(this)->debug_impl(text.c_str());
  }

  void debug(const char* text) {
    static_cast<T*>(this)->debug_impl(text);
  }

  void trace(const std::string& text) {
    static_cast<T*>(this)->trace_impl(text.c_str());
  }

  void trace(const char* text) {
    static_cast<T*>(this)->trace_impl(text);
  }

  friend T;
  void fatal_impl(const char* text) = delete;
  void error_impl(const char* text) = delete;
  void warn_impl(const char* text) = delete;
  void info_impl(const char* text) = delete;
  void debug_impl(const char* text) = delete;
  void trace_impl(const char* text) = delete;
};
} // namespace interface

#endif
