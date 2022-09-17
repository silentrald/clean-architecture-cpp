#include "./singleton.hpp"

std::unique_ptr<db::User> user_db = nullptr;
std::unique_ptr<std::mutex> get_user_db_mutex = std::make_unique<std::mutex>();

db::User* db::get_user_db() {
  if (user_db == nullptr) {
    get_user_db_mutex->lock();
    if (user_db == nullptr) {
      user_db = std::make_unique<db::UserPostgres>(db::get_postgres());
    }
    get_user_db_mutex->unlock();
  }

  return user_db.get();
}

