#include "./singleton.hpp"

// TODO: Create a throwable readable error
std::unique_ptr<db::DefUser> db::user_db = nullptr;
std::mutex db::get_user_db_mutex;

