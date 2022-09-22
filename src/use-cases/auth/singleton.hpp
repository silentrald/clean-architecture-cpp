#ifndef AUTH_USE_CASE_HPP
#define AUTH_USE_CASE_HPP

#include "./login.hpp"
#include "./logout.hpp"
#include "./user.hpp"
#include "interfaces/logger/dev.hpp"
#include "interfaces/logger/singleton.hpp"
#include <memory>
#include <mutex>

namespace use_case {
UserAuthUseCase<>* get_user_auth();
LoginAuthUseCase<>* login_auth();
LogoutAuthUseCase<>* logout_auth();
} // namespace use_case

#endif

