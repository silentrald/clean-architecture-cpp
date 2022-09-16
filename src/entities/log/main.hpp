#ifndef LOG_ENTITY_MAIN_HPP
#define LOG_ENTITY_MAIN_HPP

#include <string>
#include <vector>

namespace entity {

/**
 * Used to detailed logging
 *
 * @property msg - General Message
 * @property file - Current file which Log obj is created
 * @property function - Current function which Log obj is created
 * @property db - Db names related
 * @property errors - Used for validation errors
 */
struct Log {
  std::string msg;
  std::string file;
  std::string function;
  std::string db; // Db Names
  std::vector<std::string> errors;

  [[nodiscard("Log the error")]] std::string to_string() const {
    std::string str{};

    if (!this->file.empty()) {
      str.append("\n> File: " + this->file);
    }

    if (!this->function.empty()) {
      str.append("\n> Function: " + this->function);
    }

    if (!this->db.empty()) {
      str.append("\n> Db: " + this->db);
    }

    if (!this->errors.empty()) {
      str.append("\n> Errors:");
      for (const auto& err : this->errors) {
        str.append("\n==> " + err);
      }
    }

    if (!this->msg.empty()) {
      str.append("\n> Message: " + this->msg);
    }

    return str;
  }
};

} // namespace entity

#endif

