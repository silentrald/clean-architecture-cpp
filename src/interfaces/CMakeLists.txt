
# === LIBRARIES ===
# TODO: Check for debug or release
add_library(crypto_interface crypto/main.cpp crypto/singleton.cpp)
add_library(logger_interface logger/dev.cpp logger/singleton.cpp)
add_library(store_interface store/redis.cpp store/singleton.cpp)

# === SOME LINKERS ===
target_link_libraries(crypto_interface
  PRIVATE utils
  PRIVATE argon2
  PRIVATE logger_interface
)
target_link_libraries(store_interface
  PRIVATE utils
  PRIVATE hiredis
  PRIVATE logger_interface
)

# === TESTS ===
add_executable(crypto_interface_test crypto/test.cpp)
target_link_libraries(crypto_interface_test
  PRIVATE utils
  PRIVATE crypto_interface
  PRIVATE Catch2::Catch2
  PRIVATE Catch2::Catch2WithMain
)

add_executable(store_interface_test store/test.cpp)
target_link_libraries(store_interface_test
  PRIVATE utils
  PRIVATE hiredis
  PRIVATE store_interface
  PRIVATE Catch2::Catch2
  PRIVATE Catch2::Catch2WithMain
)

