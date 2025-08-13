#include "core/sha1.h"

#include <cstring>

namespace cactus {

SHA1 SHA1::copy(const std::uint8_t *h) {
  SHA1 res;
  std::memcpy(res.sha1.data(), h, res.hash_size);
  return res;
}

SHA1 SHA1::compute(const void *start, std::size_t n) {
  SHA1 res;
  ::SHA1(static_cast<const std::uint8_t *>(start), n, res.sha1.data());
  return res;
}

std::string SHA1::hex() const {
  static const char hex_digits[] = "0123456789abcdef";

  std::string res(hash_size * 2, '\0');
  for (std::size_t i = 0; i < hash_size; i++) {
    res[2 * i] = hex_digits[(sha1[i] >> 4) & 0xF];
    res[2 * i + 1] = hex_digits[sha1[i] & 0xF];
  }
  return res;
}

const std::uint8_t *SHA1::begin() const noexcept { return sha1.begin(); }
const std::uint8_t *SHA1::end() const noexcept { return sha1.end(); }

bool SHA1::operator==(const SHA1 &other) const noexcept {
  return sha1 == other.sha1;
}

bool SHA1::operator!=(const SHA1 &other) const noexcept {
  return sha1 != other.sha1;
}


} // namespace cactus
