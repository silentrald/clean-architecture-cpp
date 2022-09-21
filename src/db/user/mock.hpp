#ifndef USER_DB_MOCK_HPP
#define USER_DB_MOCK_HPP

#include "./build.hpp"
#include "entities/log/main.hpp"
#include "tl/expected.hpp"
#include <memory>

namespace db {

struct MockUserConfig {
  tl::expected<std::shared_ptr<entity::User>, entity::Log> user = nullptr;
};

class MockUser : public IUser<MockUser> {
private:
  MockUserConfig config;

public:
  explicit MockUser(const MockUserConfig& config) : config(config) {}

  tl::expected<std::shared_ptr<entity::User>, entity::Log>
  get_user_by_username_impl(std::string& username) noexcept {
    return this->config.user;
  }
};

} // namespace db

#endif

