

# === LIBRARIES ===
add_library(auth_use_case auth/singleton.cpp)

# === LINKERS ===
target_link_libraries(auth_use_case
  PRIVATE http_adapter
  PRIVATE hiredis
  PRIVATE store_interface
  PRIVATE db_core
  PRIVATE user_db
  PRIVATE logger_interface
)

# === TESTS ===
add_executable(auth_use_case_test auth/test.cpp)
target_link_libraries(auth_use_case_test
  PRIVATE auth_use_case
  PRIVATE http_adapter
  PRIVATE http_adapter_mock
  PRIVATE hiredis
  PRIVATE store_interface
  PRIVATE db_core
  PRIVATE user_db
  PRIVATE logger_interface
  PRIVATE Catch2::Catch2
  PRIVATE Catch2::Catch2WithMain
)

