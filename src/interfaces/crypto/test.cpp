#include "./singleton.hpp"
#include "catch2/catch_message.hpp"
#include "catch2/catch_test_macros.hpp"
#include <catch2/catch_all.hpp>

// NOLINTNEXTLINE (cert-err58-cpp)
TEST_CASE("Crypto Interface", "Interface") {
  // Create the class with default setting
  auto* crypto = interface::get_crypto();

  SECTION("Test Hash and Verify Functions") {
    std::string text = "some-random-text";
    auto exp_hashed = crypto->hash(text);
    REQUIRE(exp_hashed);

    auto& hashed = *exp_hashed;
    REQUIRE(text != hashed);

    auto exp_verified = crypto->verify(text, hashed);
    REQUIRE(exp_verified);

    REQUIRE(*exp_verified);

    text = "not-the-original";
    exp_verified = crypto->verify(text, hashed);
    REQUIRE(exp_verified);

    REQUIRE_FALSE(*exp_verified);
  }
}

