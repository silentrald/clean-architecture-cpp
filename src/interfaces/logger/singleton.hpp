#ifndef LOGGER_INTERFACE_SINGLETON_HPP
#define LOGGER_INTERFACE_SINGLETON_HPP

#include "interfaces/logger/build.hpp"
#include <memory>

#if 1 // check if debug
#include "./dev.hpp"
#else
// TODO: Create prod logger
#endif

namespace interface {
#if 1
using DefLogger = DevLogger;

std::unique_ptr<DefLogger> logger_helper() noexcept;
const std::unique_ptr<DefLogger> logger = logger_helper();

template <typename Logger = DevLogger>
ILogger<Logger>* get_logger() noexcept {
  return logger.get();
}
#else

#endif
} // namespace interface

#endif
