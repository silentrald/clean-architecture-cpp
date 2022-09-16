#!/bin/bash

# Third party
if [[ ! -d "external" ]]; then
  printf "> Creating external folder"
  mkdir external
fi
cd external

# Update function
# param 1 - Github link
# param 2 - Branch
# param 3 - Name
# param 4 - Short Description
install_module() {
  printf "===== \n"
  if [[ ! -d "$3/.git" ]]; then
    printf "> Adding \033[34;1m$3\033[0m ($4)\n"
    git clone $1 -b $2 --single-branch $3
  else
    printf "> Updating \033[34;1m$3\033[0m ($4)\n"
    cd $3
    git fetch origin $2
    cd ..
  fi
}

install_module "https://github.com/redis/hiredis.git" "master" "hiredis" "redis"
install_module "https://github.com/catchorg/Catch2.git" "devel" "catch2" "tests"
install_module "https://github.com/P-H-C/phc-winner-argon2.git" "master" "argon2" "hasher"
install_module "https://github.com/chriskohlhoff/asio.git" "master" "asio" "http"
install_module "https://github.com/nlohmann/json.git" "develop" "json" "json"
install_module "https://github.com/TartanLlama/expected.git" "master" "expected" "std::expected"

# Exit the external folder
cd ..
