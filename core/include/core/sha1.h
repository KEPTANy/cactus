#ifndef CORE_SHA1_H
#define CORE_SHA1_H

#include <array>
#include <cstdint>
#include <string>

#include "openssl/sha.h"

namespace cactus {

class SHA1 {
public:
  static const std::size_t hash_size = SHA_DIGEST_LENGTH;

  static SHA1 copy(const std::uint8_t *h);
  static SHA1 compute(const void *start, std::size_t n);

  std::string hex() const;

  const std::uint8_t *begin() const noexcept;
  const std::uint8_t *end() const noexcept;

  bool operator==(const SHA1 &) const noexcept;
  bool operator!=(const SHA1 &) const noexcept;

private:
  SHA1() = default;

  std::array<std::uint8_t, hash_size> sha1;
};

} // namespace cactus

#endif
