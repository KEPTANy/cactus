#include "core/sha1.h"

#include <cstring>

namespace cactus {

SHA1::SHA1(const std::uint8_t *h) { std::memcpy(sha1.data(), h, sha1.size()); }

SHA1::SHA1(const void *start, std::size_t n) {
  ::SHA1(static_cast<const std::uint8_t *>(start), n, sha1.data());
}

const std::uint8_t *SHA1::begin() const noexcept { return sha1.begin(); }

const std::uint8_t *SHA1::end() const noexcept { return sha1.end(); }

bool SHA1::operator==(const SHA1 &other) const noexcept {
  return sha1 == other.sha1;
}

bool SHA1::operator!=(const SHA1 &other) const noexcept {
  return sha1 != other.sha1;
}

}; // namespace cactus
