#ifndef CACTUS_CORE_CRYPTO_H
#define CACTUS_CORE_CRYPTO_H

#include <array>
#include <cstdint>
#include <string>

#include "openssl/sha.h"

namespace cactus {

namespace crypto {

class SHA1 {
public:
  static const std::size_t hash_size = SHA_DIGEST_LENGTH;

  static SHA1 copy(const std::uint8_t *h);
  static SHA1 compute(const void *start, std::size_t n);
  static SHA1 from_hex_string(std::string_view hex);

  std::string hex() const;

  const std::uint8_t *begin() const noexcept;
  const std::uint8_t *end() const noexcept;

  bool operator==(const SHA1 &) const noexcept;
  bool operator!=(const SHA1 &) const noexcept;

private:
  std::array<std::uint8_t, hash_size> m_sha1;
};

} // namespace crypto

} // namespace cactus

#endif
