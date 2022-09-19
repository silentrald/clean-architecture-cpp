#ifndef ENV_UTILS_HPP
#define ENV_UTILS_HPP

#include <cstdlib>
#include <cstring>
#include <fstream>
#include <string>

namespace utils::env {
/**
 * Initializes the environment variables with a .env file.
 *
 * NOTE: This must be done on start up before the threads are ran.
 *
 * @param path - path the .env file.
 *          If called in ./src/main.cpp, the relative path is ./src.
 *          Default "../.env".
 */
void init_env(const std::string& path = "../.env") noexcept;

/**
 * Gets the environment value with a key.
 * If the key does not exists return the default instead.
 *
 * @param key
 * @param def - If key does not exists, return this
 * @returns std::string - returns value if key exists else def
 */
std::string get_env(const std::string& key, const std::string& def) noexcept;
std::string get_env(const char* key, const char* def) noexcept;
} // namespace utils::env

#endif
