

# === LIBRARIES ===
add_library(http_adapter http/wrapper.cpp)

# === LINKERS ===
target_link_libraries(http_adapter
  PRIVATE hiredis
)

# === MOCKS ===
add_library(http_adapter_mock http/mock.cpp)
target_link_libraries(http_adapter_mock
  PRIVATE hiredis
  PRIVATE http_adapter
)

# === TESTS ===

