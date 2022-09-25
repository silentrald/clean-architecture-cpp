#ifndef DB_CORE_PG_HPP
#define DB_CORE_PG_HPP

#include "entities/log/main.hpp"
#include "interfaces/logger/singleton.hpp"
#include "tl/expected.hpp"
#include <cstddef>
#include <deque>
#include <exception>
#include <functional>
#include <libpq-fe.h>
#include <memory>
#include <mutex>
#include <optional>
#include <semaphore>
#include <string>
#include <tuple>
#include <vector>

namespace db {

/**
 * A wrapper for the postgres result class
 */
class PostgresResult {
private:
  PGresult* result;

public:
  PostgresResult() = delete;
  PostgresResult(const PostgresResult&) = delete;
  PostgresResult(PostgresResult&&) = delete;
  PostgresResult& operator=(const PostgresResult&) = delete;
  PostgresResult& operator=(PostgresResult&&) = delete;

  explicit PostgresResult(PGresult* result);
  ~PostgresResult();

  int get_count();

  template <typename T> T get_cell(int row, int col);
  // NOTE: Add support?
  /* template <typename T, typename... Args> */
  /* std::tuple<T, Args...> get_row(int row); */
};

/**
 * A wrapper for the client that can be used in transactions
 */
class PostgresClient {
private:
  PGconn* client;

public:
  PostgresClient();

  tl::expected<std::shared_ptr<PostgresResult>, entity::Log> query(
      const std::string& statement, const std::vector<std::string>& values,
      const std::vector<int>& formats
  ) noexcept;
};

struct PostgresStatement {
  std::string name;
  std::string query;
  std::vector<uint> param_types;
  size_t num_params;
};

/**
 * Main class for querying to postgres
 */
class Postgres {
private:
  std::deque<PGconn*> pool;
  /* interface::Logger* logger; */
  std::counting_semaphore<> semaphore;
  std::optional<entity::Log> error = std::nullopt;

  PGconn* get_client();
  void release_client(PGconn* client);

  int statement_count = 0;
  std::mutex count_mutex;
  std::string get_name();

  PGresult* query_with_fallback(
      const PostgresStatement& statement,
      const std::vector<std::string>& values, const std::vector<int>& formats
  );

public:
  Postgres() = delete;
  Postgres(const Postgres&) = delete;
  Postgres(Postgres&&) = delete;
  Postgres& operator=(const Postgres&) = delete;
  Postgres& operator=(Postgres&&) = delete;

  Postgres(
      const std::string& host, const std::string& port, const std::string& user,
      const std::string& password, const std::string& db, uint num_pool = 1U
      /* interface::Logger* logger = interface::get_logger() */
  );
  ~Postgres();

  tl::expected<std::shared_ptr<PostgresResult>, entity::Log> query(
      const PostgresStatement& statement,
      const std::vector<std::string>& values, const std::vector<int>& formats
  ) noexcept;

  tl::expected<PostgresStatement, entity::Log> prepare(
      const std::string& query, size_t num_params,
      const std::vector<uint>& param_types
  ) noexcept;

  // TODO: Implement with callback
  void begin_transaction() noexcept;

  // Error handling
  [[nodiscard]] std::optional<entity::Log> get_error() const noexcept;
};

} // namespace db

#endif
