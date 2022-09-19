#include "./mock.hpp"

namespace interface {

tl::expected<std::string, entity::Log>
MockCrypto::hash_impl(std::string& password) { // NOLINT
  return this->config.hashed;
}

tl::expected<bool, entity::Log>
MockCrypto::verify_impl(std::string& password, std::string& hashed) { // NOLINT
  return this->config.verified;
}

} // namespace interface

