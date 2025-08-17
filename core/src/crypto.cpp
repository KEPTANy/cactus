#include "core/crypto.h"

#include <cstring>
#include <stdexcept>

namespace cactus {

namespace crypto {

SHA1 SHA1::copy(const std::uint8_t *h) {
  SHA1 res;
  std::memcpy(res.m_sha1.data(), h, res.hash_size);
  return res;
}

SHA1 SHA1::compute(const void *start, std::size_t n) {
  SHA1 res;
  ::SHA1(static_cast<const std::uint8_t *>(start), n, res.m_sha1.data());
  return res;
}

SHA1 SHA1::from_hex_string(std::string_view hex) {
  if (hex.size() != hash_size * 2) {
    throw std::runtime_error("Bad SHA1 hex string provided");
  }

  SHA1 res;
  for (std::size_t i = 0; i < 2 * hash_size; i++) {
    if (!std::isxdigit(hex[i])) {
      std::runtime_error("Non-hex digit encountered in SHA1 string");
    }

    res.m_sha1[i / 2] <<= 4;

    if ('0' <= hex[i] && hex[i] <= '9') {
      res.m_sha1[i / 2] |= hex[i] - '0';
    } else if ('A' <= hex[i] && hex[i] <= 'F') {
      res.m_sha1[i / 2] |= hex[i] - 'A' + 10;
    } else {
      res.m_sha1[i / 2] |= hex[i] - 'a' + 10;
    }
  }
  return res;
}

std::string SHA1::hex() const {
  static const char hex_digits[] = "0123456789abcdef";

  std::string res(hash_size * 2, '\0');
  for (std::size_t i = 0; i < hash_size; i++) {
    res[2 * i] = hex_digits[(m_sha1[i] >> 4) & 0xF];
    res[2 * i + 1] = hex_digits[m_sha1[i] & 0xF];
  }
  return res;
}

const std::uint8_t *SHA1::begin() const noexcept { return m_sha1.begin(); }
const std::uint8_t *SHA1::end() const noexcept { return m_sha1.end(); }

bool SHA1::operator==(const SHA1 &other) const noexcept {
  return m_sha1 == other.m_sha1;
}

bool SHA1::operator!=(const SHA1 &other) const noexcept {
  return m_sha1 != other.m_sha1;
}

} // namespace crypto

} // namespace cactus
