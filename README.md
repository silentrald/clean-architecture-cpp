# HTTP Server with CPP

## Requirements
- CPP Compiler (Clang)
- clang-tidy and clang-format (Linters)
- PostgreSQL (Database)
- Redis (Store)
- Optional
  - A Unix Machine (Just a preference)
  - Vim (Nvim)

## Installation

### Packages
This repo relies on some open source packages which are:
- [asio](https://github.com/chriskohlhoff/asio.git) (Networking)
- [hiredis](https://github.com/redis/hiredis.git) (Redis Wrapper)
- [argon2](https://github.com/P-H-C/phc-winner-argon2.git) (Hashing) > *Can be converted to libsodium if more cryptography is needed*
- [nlohmann/json](https://github.com/nlohmann/json.git) (Json)
- [tl::expected](https://github.com/TartanLlama/expected.git) (expected)
- [Catch2](https://github.com/catchorg/Catch2.git) (Tests)

A fast way to install/update these packages is to run the `install.sh` file using `bash`

```bash
path/to/project> bash install.sh
# OR #
path/to/project> sh install.sh
```

### Database
This uses `PostgreSQL` as it's database so installing the postgres is a must to get the right headers for the cpp files.

**If you want to use another relational database, you still can, you just have to replace the `db/core` to the database you want to use.**

If you have PostgreSQL installed, you have to create the database and user to connect to, a script can be ran in the terminal that should look like this:

```sql
-- This should be run as postgres admin
CREATE DATABASE sample_db;
CREATE USER sample_user WITH PASSWORD 'password';
GRANT ALL PRIVILEGES ON DATABASE sample_db TO sample_user;

-- Connect to the database
\c sample_db

-- Add support for uuid
CREATE EXTENSION IF NOT EXISTS "uuid-ossp";

-- Quits out of the terminal
\q
```
After the database and user are created, create a `.env` file and copy the contents of `example.env` and adjust the fields of **PG_<field>** depending on the names you created;


### Redis Store
Install the redis and configure the `.env`'s **REDIS_<field>**

### Compilation
To compile the code, create a `build` folder on the root folder and run **cmake** cmdlet there.

```bash
path/to/project > mkdir build
path/to/project > cd build
path/to/project/build> cmake ..
path/to/project/build> make
```

### Running
Just execute the compiled code depending on name of the project in cmake.

```bash
# eg. on this project
path/to/project/build> ./clean_architecture
```

## Unit Tests
_**TO BE ADDED**_

## Documentation

### Swagger UI (Optional)
This project uses NodeJS to simplify the work. To run the configuration file `swagger/openapi.yaml`, clone the swagger editor and then import the file.
[swagger-editor](https://github.com/swagger-api/swagger-editor)

Alternatively, you can use the online version of swagger and import the file over there
[online version](https://editor.swagger.io)

