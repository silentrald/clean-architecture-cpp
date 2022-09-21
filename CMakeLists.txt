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
set(CMAKE_CXX_FLAGS_RELEASE_INIT        "-O3 -DNDEBUG")
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
message("> Adding Source Folder")
include_directories(src)
add_subdirectory(src)

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
