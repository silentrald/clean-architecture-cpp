
# === LIBRARIES ===
add_library(user_entity user/factory.cpp)

# === TESTS ===
add_executable(user_entity_test user/test.cpp)

# === Add their linkers ===
target_link_libraries(user_entity
  PRIVATE crypto_interface
)

target_link_libraries(user_entity_test
  PRIVATE crypto_interface
  PRIVATE user_entity
  PRIVATE Catch2::Catch2
  PRIVATE Catch2::Catch2WithMain
)

