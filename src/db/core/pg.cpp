#include "./pg.hpp"
#include <libpq-fe.h>

namespace db {
// Result
PostgresResult::PostgresResult(PGresult* result) : result(result) {}

PostgresResult::~PostgresResult() {
  PQclear(this->result);
}

int PostgresResult::get_count() {
  return PQntuples(this->result);
}

template <> char* PostgresResult::get_cell(int row, int col) {
  return PQgetvalue(this->result, row, col);
}

template <> int PostgresResult::get_cell(int row, int col) {
  return std::stoi(PQgetvalue(this->result, row, col));
}

template <> long long PostgresResult::get_cell(int row, int col) {
  return std::stoll(PQgetvalue(this->result, row, col));
}

template <> float PostgresResult::get_cell(int row, int col) {
  return std::stof(PQgetvalue(this->result, row, col));
}

template <> double PostgresResult::get_cell(int row, int col) {
  return std::stod(PQgetvalue(this->result, row, col));
}

template <> bool PostgresResult::get_cell(int row, int col) {
  return std::stoi(PQgetvalue(this->result, row, col)) > 0;
}

// Client
tl::expected<std::shared_ptr<PostgresResult>, entity::Log>
PostgresClient::query(
    const std::string& statement, const std::vector<std::string>& values,
    const std::vector<int>& formats
) noexcept {
  int size = static_cast<int>(values.size());

  std::vector<int> lengths;
  std::vector<const char*> vals;
  lengths.reserve(size);
  vals.reserve(size);

  for (uint i = 0; i < size; i++) {
    lengths[i] = static_cast<int>(values[i].size());
    vals[i] = values[i].c_str();
  }

  PGresult* result = PQexecPrepared(
      this->client, statement.c_str(), size, vals.data(), lengths.data(),
      formats.data(), 0
  );

  return std::make_shared<PostgresResult>(result);
}

// Core
Postgres::Postgres(
    const std::string& host, const std::string& port, const std::string& user,
    const std::string& password, const std::string& db, uint num_pool
    /* interface::Logger* logger */
)
    : semaphore(num_pool) {

  /* if (logger == nullptr) { */
  /*   this->error = entity::Log{ */
  /*       .msg = "Logger is not initialized", */
  /*       .file = "db/core/pg.cpp", */
  /*       .function = "Postgres::Postgres"}; */
  /*   return; */
  /* } */

  std::string connection = "host=" + host + " port=" + port + " dbname=" + db +
                           " user=" + user + " password=" + password;
  for (uint i = 0U; i < num_pool; i++) {
    PGconn* client = PQconnectdb(connection.c_str());
    auto status = PQstatus(client);

    if (status != CONNECTION_OK) {
      for (auto& c : this->pool) {
        PQfinish(c);
      }
      this->error = entity::Log{
          .msg = std::string{"Postgres could not be initialize ("} +
                 PQerrorMessage(client) + ")",
          .file = "db/core/pg.cpp",
          .function = "Postgres::Postgres"};
      PQfinish(client);
      return;
    }

    this->pool.push_back(client);
  }
}

Postgres::~Postgres() {
  if (this->pool.empty()) {
    return;
  }

  /* this->logger->debug( */
  /*     "Freeing " + std::to_string(this->pool.size()) + " pg thread pools" */
  /* ); */
  for (auto& client : this->pool) {
    PQfinish(client);
  }
}

// Private
PGconn* Postgres::get_client() {
  this->semaphore.acquire();

  PGconn* client = this->pool.front();
  this->pool.pop_front();

  return client;
}

void Postgres::release_client(PGconn* client) {
  this->pool.push_back(client);

  this->semaphore.release();
}

std::string Postgres::get_name() {
  this->count_mutex.lock();
  std::string name = std::to_string(statement_count);
  this->count_mutex.unlock();

  return name;
}

PGresult* Postgres::query_with_fallback(
    const PostgresStatement& statement, const std::vector<std::string>& values,
    const std::vector<int>& formats
) {
  int size = static_cast<int>(values.size());

  std::vector<int> lengths;
  std::vector<const char*> vals;
  lengths.reserve(size);
  vals.reserve(size);

  for (uint i = 0; i < size; i++) {
    lengths[i] = static_cast<int>(values[i].size());
    vals[i] = values[i].c_str();
  }

  PGconn* client = this->get_client();
  PGresult* result = PQexecPrepared(
      client, statement.name.c_str(), size, vals.data(), lengths.data(),
      formats.data(), 0
  );

  // FALLBACK
  if (PQresultStatus(result) == PGRES_FATAL_ERROR) {
    // Check if the statement exists
    PGresult* rtmp = PQprepare(
        client, statement.name.c_str(), statement.query.c_str(),
        statement.num_params, statement.param_types.data()
    );

    // Created the prepared statement
    if (PQresultStatus(rtmp) == PGRES_COMMAND_OK) {
      PQclear(result);

      // Requery
      result = PQexecPrepared(
          client, statement.name.c_str(), size, vals.data(), lengths.data(),
          formats.data(), 0
      );
    }
    PQclear(rtmp);
  }

  this->release_client(client);

  return result;
}

tl::expected<std::shared_ptr<PostgresResult>, entity::Log> Postgres::query(
    const PostgresStatement& statement, const std::vector<std::string>& values,
    const std::vector<int>& formats
) noexcept {
  PGresult* result = this->query_with_fallback(statement, values, formats);

  const char* err = PQresultErrorMessage(result);
  if (*err != '\0') {
    entity::Log log{
        .msg = err, .file = "db/core/pg.hpp", .function = "Postgres::query"};
    PQclear(result);
    return tl::unexpected<entity::Log>(log);
  }

  return std::make_shared<PostgresResult>(result);
}

tl::expected<PostgresStatement, entity::Log> Postgres::prepare(
    const std::string& query, int num_params,
    const std::vector<uint>& param_types
) noexcept {
  return PostgresStatement{
      .name = this->get_name(),
      .query = query,
      .param_types = param_types,
      .num_params = num_params};
}
// Cache this then query
/* PGconn* client = this->get_client(); */
/* PGresult* result = */
/*     PQprepare(client, name.c_str(), query.c_str(), num_params,
 * param_types.data()); */
/* this->release_client(client); */

/* auto status = PQresultStatus(result); */

/* if (status == PGRES_COMMAND_OK) { */
/*   PQclear(result); */
/*   return name; */
/* } */

/* entity::Log log{ */
/*     .msg = PQresultErrorMessage(result), */
/*     .file = "db/core/pg.hpp", */
/*     .function = "Postgres::prepare"}; */

/* PQclear(result); */

/* return tl::unexpected<entity::Log>(log); */
/* } */

[[nodiscard]] std::optional<entity::Log> Postgres::get_error() const noexcept {
  return this->error;
}

} // namespace db

