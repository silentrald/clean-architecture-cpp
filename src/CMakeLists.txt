
#== = UTILS == =
file(GLOB UTIL_SRCS utils/*.cpp)
add_library(utils ${UTIL_SRCS})

# === SUBDIRECTORIES ===
add_subdirectory(entities)
add_subdirectory(interfaces)
add_subdirectory(db)
add_subdirectory(wrappers)
add_subdirectory(adapters)
add_subdirectory(use-cases)

# === LINKERS ===

### *== DB ==*
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

### *== ADAPTERS ==*
target_link_libraries(http_adapter
  PRIVATE hiredis
  PRIVATE store_interface
  PRIVATE http_wrapper
)

