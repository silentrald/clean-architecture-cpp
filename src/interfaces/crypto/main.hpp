#ifndef CRYPTO_INTERFACE_MAIN_HPP
#define CRYPTO_INTERFACE_MAIN_HPP

#include "./build.hpp"
#include "entities/log/main.hpp"
#include "tl/expected.hpp"
#include <argon2.h>
#include <string>
#include <string_view>
#include <sys/types.h>
#include <vector>

namespace interface {
class MainCrypto : public ICrypto<MainCrypto> {
private:
  uint iteration_cost;
  uint memory_cost;
  uint parallelism;
  uint hash_len;
  std::string salt;
  uint encoded_len;

public:
  MainCrypto(
      uint iteration_cost, uint memory_cost, uint parallelism, uint hash_len,
      std::string& salt
  );

  tl::expected<std::string, entity::Log> hash_impl(std::string& password);
  tl::expected<bool, entity::Log>
  verify_impl(std::string& password, std::string& hashed);
};
} // namespace interface

#endif

