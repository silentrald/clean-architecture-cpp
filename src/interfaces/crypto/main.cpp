#include "./main.hpp"
#include "argon2.h"
#include "tl/expected.hpp"

const char* const CURR_FILE = "interfaces/crypto/main.cpp";

namespace interface {

MainCrypto::MainCrypto(
    uint iteration_cost, uint memory_cost, uint parallelism, uint hash_len,
    std::string& salt
)
    : iteration_cost(iteration_cost),
      memory_cost(memory_cost),
      parallelism(parallelism),
      hash_len(hash_len),
      salt(salt),
      encoded_len(argon2_encodedlen(
          iteration_cost, memory_cost, parallelism, salt.size(), hash_len,
          Argon2_id
      )) {}

tl::expected<std::string, entity::Log>
MainCrypto::hash_impl(std::string& password) {
  std::vector<char> encoded;
  encoded.reserve(this->encoded_len);
  int err = argon2id_hash_encoded(
      this->iteration_cost, this->memory_cost, this->parallelism,
      password.c_str(), password.size(), this->salt.c_str(), this->salt.size(),
      this->hash_len, encoded.data(), this->encoded_len
  );

  if (err != ARGON2_OK) {
    return tl::unexpected<entity::Log>(
        {.msg = argon2_error_message(err),
         .file = CURR_FILE,
         .function = "hash_impl"}
    );
  }

  return std::string{encoded.begin(), encoded.begin() + this->encoded_len};
}

tl::expected<bool, entity::Log>
// NOLINTNEXTLINE(*-convert-member-functions-to-static)
MainCrypto::verify_impl(std::string& password, std::string& hashed) {
  int err = argon2id_verify(hashed.c_str(), password.c_str(), password.size());
  if (err == ARGON2_OK) {
    return true;
  }

  if (err == ARGON2_VERIFY_MISMATCH) {
    return false;
  }

  return tl::unexpected<entity::Log>(
      {.msg = argon2_error_message(err),
       .file = CURR_FILE,
       .function = "hash_impl"}
  );
}

} // namespace interface
