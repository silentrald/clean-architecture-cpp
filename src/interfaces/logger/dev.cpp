#include "./dev.hpp"

using namespace interface;

const char* const TEXT_WHITE = "\e[97m";
const char* const TEXT_BLACK = "\e[30m";
const char* const TEXT_RED = "\x1B[31m";
const char* const TEXT_GREEN = "\x1B[32m";
const char* const TEXT_YELLOW = "\x1B[33m";
const char* const TEXT_BLUE = "\x1B[34m";
const char* const TEXT_MAGENTA = "\e[95m";
const char* const BG_WHITE = "\e[107m";
const char* const BG_RED = "\e[41m";
const char* const TEXT_BOLD = "\033[1m";
const char* const TEXT_NORMAL = "\033[0m";
const char* const RESET = "\x1B[0m";

const int STACK_TRACE_LENGTH = 50;
const int MILLIS = 1000;

DevLogger::DevLogger(enum Logging level) : level(level) {}

std::string DevLogger::now() {
  timeval current = {};
  gettimeofday(&current, nullptr);

  uint size = strftime(
      this->date_time.data(), DATE_TIME_LENGTH,
      "%Y-%m-%dT%H:%M:%S.",
      localtime(&current.tv_sec) // NOLINT(concurrency-mt-unsafe)
  );

  if (size == 0) {
    return "";
  }

  // NOLINTNEXTLINE(cert-err33-c)
  sprintf(
      this->date_time.data(), "%s%03ld", date_time.data(),
      current.tv_usec / MILLIS
  );
  return std::string{
      this->date_time.begin(), this->date_time.begin() + DATE_TIME_LENGTH};
}

void DevLogger::fatal_impl(const char* text) {
  if (this->level < LOGGING_FATAL) {
    return;
  }

  this->logger_mutex.lock();
  printf("%s %s%s%sfatal:%s %s\n", now().c_str(), TEXT_BOLD, TEXT_BLACK, BG_RED, RESET, text);
  this->logger_mutex.unlock();
}

void DevLogger::error_impl(const char* text) {
  if (this->level < LOGGING_ERROR) {
    return;
  }

  this->logger_mutex.lock();
  printf(
      "%s %s%serror:%s %s\n", now().c_str(), TEXT_BOLD, TEXT_RED, RESET, text
  );
  this->logger_mutex.unlock();
}

void DevLogger::warn_impl(const char* text) {
  if (this->level < LOGGING_WARN) {
    return;
  }

  this->logger_mutex.lock();
  printf(
      "%s %s%swarn:%s %s\n", now().c_str(), TEXT_BOLD, TEXT_YELLOW, RESET, text
  );
  this->logger_mutex.unlock();
}

void DevLogger::info_impl(const char* text) {
  if (this->level < LOGGING_INFO) {
    return;
  }

  this->logger_mutex.lock();
  printf(
      "%s %s%sinfo:%s %s\n", now().c_str(), TEXT_BOLD, TEXT_GREEN, RESET, text
  );
  this->logger_mutex.unlock();
}

void DevLogger::debug_impl(const char* text) {
  if (this->level < LOGGING_DEBUG) {
    return;
  }

  this->logger_mutex.lock();
  printf(
      "%s %s%sdebug:%s %s\n", now().c_str(), TEXT_BOLD, TEXT_MAGENTA, RESET,
      text
  );
  this->logger_mutex.unlock();
}

void DevLogger::trace_impl(const char* text) {
  if (this->level < LOGGING_TRACE) {
    return;
  }

  this->logger_mutex.lock();
  printf(
      "%s %s%strace:%s %s\n", now().c_str(), TEXT_BOLD, TEXT_BLUE, RESET, text
  );
  this->logger_mutex.unlock();
}

