

# === LIBRARIES ===
# Postgres Config
add_library(db_core core/pg.cpp core/singleton.cpp)

add_library(user_db user/pg.cpp user/singleton.cpp)
target_link_libraries(user_db PRIVATE db_core)

# === TESTS ===

