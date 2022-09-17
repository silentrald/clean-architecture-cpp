#include "./singleton.hpp"

#if 1
// TODO: Changeable on env
const std::unique_ptr<interface::Logger> logger =
    std::make_unique<interface::DevLogger>(interface::LOGGING_INFO);

interface::Logger* interface::get_logger() {
  return logger.get();
}
#else
// TODO: Create prod here
#endif
