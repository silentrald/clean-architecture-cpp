#ifndef CRYPTO_INTERFACE_MOCK_HPP
#define CRYPTO_INTERFACE_MOCK_HPP

#include "./build.hpp"
#include "entities/log/main.hpp"
#include "tl/expected.hpp"

namespace interface {

struct MockCryptoConfig {
  tl::expected<std::string, entity::Log> hashed;
  tl::expected<bool, entity::Log> verified;
};

class MockCrypto : public ICrypto<MockCrypto> {
private:
  MockCryptoConfig config;

public:
  explicit MockCrypto(const MockCryptoConfig&& config) : config(config) {}

  tl::expected<std::string, entity::Log> hash_impl(std::string& password);
  tl::expected<bool, entity::Log>
  verify_impl(std::string& password, std::string& hashed);
};

} // namespace interface

#endif

