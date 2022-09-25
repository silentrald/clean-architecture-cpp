#ifndef AUTH_USE_CASE_HPP
#define AUTH_USE_CASE_HPP

#include "./login.hpp"
#include "./logout.hpp"
#include "./user.hpp"
#include "interfaces/logger/dev.hpp"
#include "interfaces/logger/singleton.hpp"
#include "use-cases/auth/register.hpp"
#include <memory>
#include <mutex>

namespace use_case {
UserAuthUseCase<>* get_user_auth() noexcept;
LoginAuthUseCase<>* login_auth() noexcept;
LogoutAuthUseCase<>* logout_auth() noexcept;
RegisterAuthUseCase<>* register_auth() noexcept;
} // namespace use_case

#endif

