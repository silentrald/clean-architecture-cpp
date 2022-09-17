#include "./singleton.hpp"
#include "utils/env.hpp"

std::unique_ptr<interface::Crypto> crypto = nullptr;
std::mutex crypto_mutex;

const char* const DEFAULT_ITERATION_COST = "2";
const char* const DEFAULT_MEMORY_COST = "16348";
const char* const DEFAULT_PARALLELISM = "1";
const char* const DEFAULT_HASH_LEN = "32";
const char* const DEFAULT_SECRET = "super-secret-password";

interface::Crypto* interface::get_crypto() {
  if (crypto == nullptr) {
    crypto_mutex.lock();
    if (crypto == nullptr) {
      std::string salt = utils::env::get_env("ARGONID_SECRET", DEFAULT_SECRET);
      crypto = std::make_unique<interface::MainCrypto>(
          std::stoul(utils::env::get_env(
              "ARGONID_ITERATION_COST", DEFAULT_ITERATION_COST
          )),
          std::stoul(
              utils::env::get_env("ARGONID_MEMORY_COST", DEFAULT_MEMORY_COST)
          ),
          std::stoul(
              utils::env::get_env("ARGONID_PARALLELISM", DEFAULT_PARALLELISM)
          ),
          std::stoul(utils::env::get_env("ARGONID_HASH_LEN", DEFAULT_HASH_LEN)),
          salt
      );
    }
    crypto_mutex.unlock();
  }

  return crypto.get();
}

