#ifndef CRYPTO_INTERFACE_BUILD_HPP
#define CRYPTO_INTERFACE_BUILD_HPP

#include "entities/log/main.hpp"
#include <string>
#include <tl/expected.hpp>

namespace interface {
template <typename T> class ICrypto {
private:
  ICrypto() = default;

public:
  tl::expected<std::string, entity::Log> hash(std::string& text) {
    return static_cast<T*>(this)->hash_impl(text);
  }

  tl::expected<bool, entity::Log>
  verify(std::string& text, std::string& hashed) {
    return static_cast<T*>(this)->verify_impl(text, hashed);
  }
  friend T;

  tl::expected<std::string, entity::Log> hash_impl(std::string& text) = delete;
  tl::expected<bool, entity::Log>verify_impl(std::string& text, std::string& hashed) = delete;
};
} // namespace interface

#endif

