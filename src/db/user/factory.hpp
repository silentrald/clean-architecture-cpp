#ifndef USER_DB_MAIN_HPP
#define USER_DB_MAIN_HPP

#include "./pg.hpp"
#include "../core/singleton.hpp"
#include <memory>

namespace db {
using User = IUser<UserPostgres>;
User* get_user_db();
} // namespace db

#endif
