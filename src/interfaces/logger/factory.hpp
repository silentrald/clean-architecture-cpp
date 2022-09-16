#ifndef LOGGER_INTERFACE_MAIN_HPP
#define LOGGER_INTERFACE_MAIN_HPP

#include <memory>

#if 1 // check if debug
#include "./dev.hpp"
#else
// TODO: Create prod logger
#endif

namespace interface {
#if 1
using Logger = ILogger<DevLogger>;
Logger* get_logger();
#else

#endif
} // namespace interface

#endif
