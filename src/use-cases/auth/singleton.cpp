#include "./singleton.hpp"

std::unique_ptr<use_case::LoginAuthUseCase> login_auth_ptr = nullptr;
std::mutex login_auth_mutex;

use_case::LoginAuthUseCase* use_case::login_auth() {
  if (login_auth_ptr == nullptr) {
    login_auth_mutex.lock();
    if (login_auth_ptr == nullptr) {
      login_auth_ptr = std::make_unique<use_case::LoginAuthUseCase>(
          db::get_user_db(), interface::get_store(), interface::get_logger()
      );
    }
    login_auth_mutex.unlock();
  }

  return login_auth_ptr.get();
}

std::unique_ptr<use_case::LogoutAuthUseCase> logout_auth_ptr = nullptr;
std::mutex logout_auth_mutex;

use_case::LogoutAuthUseCase* use_case::logout_auth() {
  if (logout_auth_ptr == nullptr) {
    logout_auth_mutex.lock();
    if (logout_auth_ptr == nullptr) {
      logout_auth_ptr =
          std::make_unique<use_case::LogoutAuthUseCase>(interface::get_logger()
          );
    }
    logout_auth_mutex.unlock();
  }

  return logout_auth_ptr.get();
}
