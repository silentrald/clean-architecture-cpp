cmake_minimum_required(VERSION 3.22.1)
project(clean_architecture)

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++2b")

set(CMAKE_C_FLAGS_DEBUG_INIT          "-g")
set(CMAKE_C_FLAGS_MINSIZEREL_INIT     "-Os -DNDEBUG")
set(CMAKE_C_FLAGS_RELEASE_INIT        "-O3 -DNDEBUG")
set(CMAKE_C_FLAGS_RELWITHDEBINFO_INIT "-O2 -g")

set(CMAKE_CXX_FLAGS_DEBUG_INIT          "-g")
set(CMAKE_CXX_FLAGS_MINSIZEREL_INIT     "-Os -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE_INIT        "-O2 -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO_INIT "-O2 -g")

message("===== BUILING =====")
message("> CC Compiler:  ${CMAKE_C_COMPILER_ID}")
message("> CXX Compiler: ${CMAKE_CXX_COMPILER_ID}")

add_executable(${PROJECT_NAME} src/main.cpp)

# 3rd party
add_subdirectory(external/hiredis)
add_subdirectory(external/catch2)

include_directories(external/argon2/include)
include_directories(external/asio/asio/include)
include_directories(external/json/single_include)
include_directories(external/expected/include)

# Source Folder
include_directories(src)
# message("-- Adding Source Folder")
# add_subdirectory(src)

# Utils
file(GLOB UTIL_SRCS
  src/utils/*.cpp
)
add_library(utils ${UTIL_SRCS})

# Entities
file(GLOB USER_ENTITY_SRCS
  src/entities/user/*.cpp
)
add_library(user_entity ${USER_ENTITY_SRCS})

# DB
file(GLOB DB_CORE_SRCS
  src/db/core/*.cpp
)
add_library(db_core ${DB_CORE_SRCS})

file(GLOB USER_DB_SRCS
  src/db/user/*.cpp
)
add_library(user_db ${USER_DB_SRCS})

# Wrappers
file(GLOB HTTP_WRAPPER_SRCS
  src/wrappers/http/*.cpp
)
add_library(http_wrapper ${HTTP_WRAPPER_SRCS})

# Adapter
file(GLOB HTTP_ADAPTER_SRCS
  src/adapter/http/*.cpp
)
add_library(http_adapter ${HTTP_ADAPTER_SRCS})

# Interfaces
file(GLOB LOGGER_INTERFACE_SRCS
  src/interfaces/logger/*.cpp
)
add_library(logger_interface ${LOGGER_INTERFACE_SRCS})
 
file(GLOB CRYPTO_INTERFACE_SRCS
  src/interfaces/crypto/*.cpp
)
add_library(crypto_interface ${CRYPTO_INTERFACE_SRCS})

file(GLOB STORE_INTERFACE_SRCS
  src/interfaces/store/*.cpp
)
add_library(store_interface ${STORE_INTERFACE_SRCS})

# Use Cases
file(GLOB AUTH_USE_CASE_SRCS
  src/use-cases/auth/*.cpp
)
add_library(auth_use_case ${AUTH_USE_CASE_SRCS})

target_link_libraries(crypto_interface
  PRIVATE utils
  PRIVATE argon2
)

target_link_libraries(store_interface
  PRIVATE hiredis
  PRIVATE logger_interface
)

target_link_libraries(db_core
  PRIVATE pq
  PRIVATE utils
  PRIVATE logger_interface
)

target_link_libraries(user_db
  PRIVATE db_core
  PRIVATE user_entity
  PRIVATE logger_interface
)

target_link_libraries(user_entity
  PRIVATE Catch2::Catch2
  PRIVATE Catch2::Catch2WithMain
)

target_link_libraries(auth_use_case
  PRIVATE hiredis
  PRIVATE db_core
  PRIVATE http_adapter
  PRIVATE db_core
  PRIVATE user_db
  PRIVATE store_interface
  PRIVATE logger_interface
)

target_link_libraries(http_adapter
  PRIVATE hiredis
  PRIVATE store_interface
  PRIVATE http_wrapper
)

target_link_libraries(${PROJECT_NAME}
  PRIVATE hiredis
  PRIVATE utils
  PRIVATE crypto_interface
  PRIVATE user_entity
  PRIVATE user_db
  PRIVATE http_wrapper
  PRIVATE http_adapter
  PRIVATE auth_use_case
  PRIVATE logger_interface
  PRIVATE store_interface
)
