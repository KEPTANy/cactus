#ifndef CORE_SHA1_H
#define CORE_SHA1_H

#include <array>
#include <cstdint>

#include "openssl/sha.h"

namespace cactus {

class SHA1 {
public:
  static const std::size_t hash_size = SHA_DIGEST_LENGTH;

  // Copies the hash that is stored staring at provided ptr
  SHA1(const std::uint8_t *h);

  // Hashes n bytes
  SHA1(const void *start, std::size_t n);

  const std::uint8_t *begin() const noexcept;
  const std::uint8_t *end() const noexcept;

  bool operator==(const SHA1 &) const noexcept;
  bool operator!=(const SHA1 &) const noexcept;

private:
  std::array<std::uint8_t, hash_size> sha1;
};

}; // namespace cactus

#endif
