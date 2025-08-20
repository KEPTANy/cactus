#ifndef CACTUS_CORE_PIECE_H
#define CACTUS_CORE_PIECE_H

#include <cstdint>
#include <vector>

#include "core/crypto.h"

namespace cactus {

class Piece {
public:
  Piece(std::size_t size, std::size_t offset);

  crypto::SHA1 sha1() const;

  std::size_t size() const noexcept;
  std::size_t offset() const noexcept;
  std::vector<std::uint8_t> &vec();
  const std::vector<std::uint8_t> &vec() const;

private:
  std::vector<std::uint8_t> m_bytes;
  std::size_t m_offset;
};

} // namespace cactus

#endif
