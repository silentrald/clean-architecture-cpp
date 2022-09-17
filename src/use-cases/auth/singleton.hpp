#ifndef AUTH_USE_CASE_HPP
#define AUTH_USE_CASE_HPP

#include "./login.hpp"
#include "./logout.hpp"
#include <memory>
#include <mutex>

namespace use_case {
LoginAuthUseCase* login_auth();
LogoutAuthUseCase* logout_auth();
} // namespace use_case

#endif

