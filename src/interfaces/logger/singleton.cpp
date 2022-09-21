#include "./singleton.hpp"
#include <iostream>
#include <memory>

#if 1

std::unique_ptr<interface::DefLogger> interface::logger_helper() noexcept {
  try {
    extern std::unique_ptr<DefLogger> f() noexcept(false);
    return std::make_unique<DevLogger>(interface::LOGGING_TRACE);
  } catch (...) {
    extern void log_message(const char*) noexcept;
    std::cerr << "interface::logger_helper() threw an exception\n";
    std::terminate();
  }
}

#else
// TODO: Create prod here
#endif
