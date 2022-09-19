#include "./env.hpp"

namespace env = utils::env;

// NOTE: Concurrency will not matter since init is ran
// before the threads are executed and after that, env will only be read

void env::init_env(const std::string& path) noexcept {
  std::ifstream file(path);
  if (!file.is_open()) {
    // BUG: SILENT ERROR
    return;
  }

  std::string line;
  while (std::getline(file, line)) {
    if (line.empty() || line.at(0) == '#') {
      continue;
    }

    size_t pos = line.find('=');

    // NOLINTNEXTLINE(concurrency-mt-unsafe)
    setenv(line.substr(0, pos).c_str(), line.substr(pos + 1).c_str(), 1);
  }
  file.close();
}

std::string env::get_env(const std::string& key, const std::string& def) noexcept {
  // NOLINTNEXTLINE(concurrency-mt-unsafe)
  const char* env = getenv(key.c_str());
  if (env == nullptr || strcmp(env, "") == 0) {
    return def;
  }

  // NOLINTNEXTLINE(concurrency-mt-unsafe)
  return std::string{env};
}

std::string env::get_env(const char* key, const char* def) noexcept {
  // NOLINTNEXTLINE(concurrency-mt-unsafe)
  const char* env = getenv(key);
  if (env == nullptr || strcmp(env, "") == 0) {
    return std::string{def};
  }

  // NOLINTNEXTLINE(concurrency-mt-unsafe)
  return std::string{env};
}

