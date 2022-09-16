#ifndef CRYPTO_INTERFACE_FACTORY_HPP
#define CRYPTO_INTERFACE_FACTORY_HPP

#include "./main.hpp"
#include <memory>
#include <mutex>

namespace interface {
using Crypto = ICrypto<MainCrypto>;

Crypto* get_crypto();
} // namespace interface

#endif

