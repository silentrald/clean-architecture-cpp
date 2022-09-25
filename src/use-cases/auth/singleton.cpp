#include "./singleton.hpp"
#include "db/user/singleton.hpp"
#include "interfaces/logger/singleton.hpp"
#include "use-cases/auth/register.hpp"
#include "use-cases/auth/user.hpp"
#include <memory>

// TODO: Can throw if cannot allocate the pointers

std::unique_ptr<use_case::LoginAuthUseCase<>> login_auth_ptr = nullptr;
std::mutex login_auth_mutex;

std::unique_ptr<use_case::LogoutAuthUseCase<>> logout_auth_ptr = nullptr;
std::mutex logout_auth_mutex;

std::unique_ptr<use_case::UserAuthUseCase<>> get_user_auth_ptr = nullptr;
std::mutex get_user_auth_mutex;

std::unique_ptr<use_case::RegisterAuthUseCase<>> register_auth_ptr = nullptr;
std::mutex register_auth_mutex;

use_case::LoginAuthUseCase<>* use_case::login_auth() noexcept {
  if (login_auth_ptr == nullptr) {
    login_auth_mutex.lock();
    if (login_auth_ptr == nullptr) {
      login_auth_ptr = std::make_unique<use_case::LoginAuthUseCase<>>(
          db::get_user_db(), interface::get_store(), interface::get_logger()
      );
    }
    login_auth_mutex.unlock();
  }

  return login_auth_ptr.get();
}

use_case::LogoutAuthUseCase<>* use_case::logout_auth() noexcept {
  if (logout_auth_ptr == nullptr) {
    logout_auth_mutex.lock();
    if (logout_auth_ptr == nullptr) {
      logout_auth_ptr = std::make_unique<use_case::LogoutAuthUseCase<>>(
          interface::get_logger()
      );
    }
    logout_auth_mutex.unlock();
  }

  return logout_auth_ptr.get();
}

use_case::UserAuthUseCase<>* use_case::get_user_auth() noexcept {
  if (get_user_auth_ptr == nullptr) {
    get_user_auth_mutex.lock();
    if (get_user_auth_ptr == nullptr) {
      get_user_auth_ptr =
          std::make_unique<use_case::UserAuthUseCase<>>(interface::get_logger()
          );
    }
    get_user_auth_mutex.unlock();
  }

  return get_user_auth_ptr.get();
}

use_case::RegisterAuthUseCase<>* use_case::register_auth() noexcept {
  if (register_auth_ptr == nullptr) {
    register_auth_mutex.lock();
    if (register_auth_ptr == nullptr) {
      register_auth_ptr = std::make_unique<use_case::RegisterAuthUseCase<>>(
          db::get_user_db(), interface::get_logger()
      );
    }
    register_auth_mutex.unlock();
  }

  return register_auth_ptr.get();
}

