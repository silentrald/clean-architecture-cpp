#include "./factory.hpp"

#if 1
const std::unique_ptr<interface::Logger> logger =
    std::make_unique<interface::DevLogger>(interface::LOGGING_FATAL);

interface::Logger* interface::get_logger() {
  return logger.get();
}
#else
// TODO: Create prod here
#endif
