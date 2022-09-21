#ifndef USER_DB_SINGLETON_HPP
#define USER_DB_SINGLETON_HPP

#include "./pg.hpp"
#include "../core/singleton.hpp"
#include <memory>

namespace db {

using DefUser = UserPostgres;

extern std::unique_ptr<db::DefUser> user_db;
extern std::mutex get_user_db_mutex;

template <typename User = DefUser>
IUser<User>* get_user_db() noexcept {
  if (user_db == nullptr) {
    get_user_db_mutex.lock();
    if (user_db == nullptr) {
      user_db = std::make_unique<db::UserPostgres>(db::get_postgres());
    }
    get_user_db_mutex.unlock();
  }

  return user_db.get();
}

} // namespace db

#endif

