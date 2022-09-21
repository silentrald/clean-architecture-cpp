// NOTE: REDIS SERVER MUST BE ACTIVE

#include "./singleton.hpp"
#include "utils/number.hpp"
#include <catch2/catch_message.hpp>
#include <catch2/catch_test_macros.hpp>
#include <optional>

// NOLINTNEXTLINE
TEST_CASE("Store Interface", "Interface") {
  // Create the singleton
  auto err = interface::init_store();
  if (err) {
    INFO("Cannot init store, store must be closed");
  }
  REQUIRE(!err);

  auto* store = interface::get_store();

  SECTION("Int32 Functions") {
    bool set = store->set_int("some-key", 1);
    REQUIRE(set);

    auto exp_val = store->get_int32("some-key");
    REQUIRE(exp_val);
    auto val = *exp_val;
    REQUIRE(val);
    REQUIRE(*val == 1);

    bool exists = store->exists("some-key");
    REQUIRE(exists);

    bool deleted = store->del("some-key");
    REQUIRE(deleted);

    exists = store->exists("some-key");
    REQUIRE_FALSE(exists);
  }

  SECTION("Int64 Functions") {
    bool set = store->set_int64("some-key", 1LL);
    REQUIRE(set);

    auto val = store->get_int64("some-key");
    REQUIRE(val);
    REQUIRE(*val == 1LL);

    bool exists = store->exists("some-key");
    REQUIRE(exists);

    bool deleted = store->del("some-key");
    REQUIRE(deleted);

    exists = store->exists("some-key");
    REQUIRE_FALSE(exists);
  }

  SECTION("Float Functions") {
    const float flt = 1.1F;
    bool set = store->set_float("some-key", flt);
    REQUIRE(set);

    auto val = store->get_float("some-key");
    REQUIRE(val);
    REQUIRE(*val);
    REQUIRE(utils::number::compare_float(**val, flt));

    bool exists = store->exists("some-key");
    REQUIRE(exists);

    bool deleted = store->del("some-key");
    REQUIRE(deleted);

    exists = store->exists("some-key");
    REQUIRE_FALSE(exists);
  }

  SECTION("Double Functions") {
    const double dbl = 1.1;
    bool set = store->set_double("some-key", dbl);
    REQUIRE(set);

    auto val = store->get_double("some-key");
    REQUIRE(val);
    REQUIRE(*val);
    REQUIRE(utils::number::compare_double(**val, dbl));

    bool exists = store->exists("some-key");
    REQUIRE(exists);

    bool deleted = store->del("some-key");
    REQUIRE(deleted);

    exists = store->exists("some-key");
    REQUIRE_FALSE(exists);
  }

  SECTION("Bool(true) Functions") {
    bool set = store->set_bool("some-key", true);
    REQUIRE(set);

    auto val = store->get_bool("some-key");
    REQUIRE(val);
    REQUIRE(*val);
    REQUIRE(**val);

    bool exists = store->exists("some-key");
    REQUIRE(exists);

    bool deleted = store->del("some-key");
    REQUIRE(deleted);

    exists = store->exists("some-key");
    REQUIRE_FALSE(exists);
  }

  SECTION("Bool(false) Functions") {
    bool set = store->set_bool("some-key", false);
    REQUIRE(set);

    auto val = store->get_bool("some-key");
    REQUIRE(val);
    REQUIRE(*val);
    REQUIRE_FALSE(**val);

    bool exists = store->exists("some-key");
    REQUIRE(exists);

    bool deleted = store->del("some-key");
    REQUIRE(deleted);

    exists = store->exists("some-key");
    REQUIRE_FALSE(exists);
  }

  SECTION("String Functions") {
    bool set = store->set_string("some-key", "some-random-string");
    REQUIRE(set);

    auto val = store->get_string("some-key");
    REQUIRE(val);
    REQUIRE(*val);
    REQUIRE(**val == "some-random-string");

    bool exists = store->exists("some-key");
    REQUIRE(exists);

    bool deleted = store->del("some-key");
    REQUIRE(deleted);

    exists = store->exists("some-key");
    REQUIRE_FALSE(exists);
  }

  SECTION("Null Int32") {
    store->del("some-key");

    auto null = store->get_int32("some-key");
    REQUIRE(null);
    REQUIRE_FALSE(*null);
  }

  SECTION("Null Int64") {
    store->del("some-key");

    auto null = store->get_int64("some-key");
    REQUIRE(null);
    REQUIRE_FALSE(*null);
  }

  SECTION("Null Float") {
    store->del("some-key");

    auto null = store->get_float("some-key");
    REQUIRE(null);
    REQUIRE_FALSE(*null);
  }

  SECTION("Null Double") {
    store->del("some-key");

    auto null = store->get_double("some-key");
    REQUIRE(null);
    REQUIRE_FALSE(*null);
  }

  SECTION("Null Bool") {
    store->del("some-key");

    auto null = store->get_bool("some-key");
    REQUIRE(null);
    REQUIRE_FALSE(*null);
  }

  SECTION("Null String") {
    store->del("some-key");

    auto null = store->get_string("some-key");
    REQUIRE(null);
    REQUIRE_FALSE(*null);
  }

  SECTION("Invalid Int32") {
    store->set_string("hello", "some-random-string");

    auto err = store->get_int32("hello");
    REQUIRE(!err);

    auto& log = err.error();
    REQUIRE(log.msg == "Could not parse to int32");
    REQUIRE(log.file == "interfaces/store/redis.cpp");
    REQUIRE(log.function == "RedisStore::get_int32_impl(const char*)");
  }

  SECTION("Invalid Int64") {
    store->set_string("hello", "some-random-string");

    auto err = store->get_int64("hello");
    REQUIRE(!err);

    auto& log = err.error();
    REQUIRE(log.msg == "Could not parse to int64");
    REQUIRE(log.file == "interfaces/store/redis.cpp");
    REQUIRE(log.function == "RedisStore::get_int64_impl(const char*)");
  }

  SECTION("Invalid Float") {
    store->set_string("hello", "some-random-string");

    auto err = store->get_float("hello");
    REQUIRE(!err);

    auto& log = err.error();
    REQUIRE(log.msg == "Could not parse to float");
    REQUIRE(log.file == "interfaces/store/redis.cpp");
    REQUIRE(log.function == "RedisStore::get_float_impl(const char*)");
  }

  SECTION("Invalid Double") {
    store->set_string("hello", "some-random-string");

    auto err = store->get_double("hello");
    REQUIRE(!err);

    auto& log = err.error();
    REQUIRE(log.msg == "Could not parse to double");
    REQUIRE(log.file == "interfaces/store/redis.cpp");
    REQUIRE(log.function == "RedisStore::get_double_impl(const char*)");
  }
}

