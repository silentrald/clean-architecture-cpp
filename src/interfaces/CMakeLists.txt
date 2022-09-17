
# === LIBRARIES ===
# TODO: Check for debug or release
add_library(crypto_interface crypto/main.cpp crypto/singleton.cpp)
add_library(logger_interface logger/dev.cpp logger/singleton.cpp)
add_library(store_interface store/redis.cpp store/singleton.cpp)

# === SOME LINKERS ===
target_link_libraries(crypto_interface
  PRIVATE logger_interface
)
target_link_libraries(store_interface
  PRIVATE logger_interface
)

# === TESTS ===

