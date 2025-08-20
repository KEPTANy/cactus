#include "core/piece.h"

namespace cactus {

Piece::Piece(std::size_t size, std::size_t offset)
    : m_offset{offset}, m_bytes(size) {}

crypto::SHA1 Piece::sha1() const {
  return crypto::SHA1::compute(m_bytes.data(), m_bytes.size());
}

std::size_t Piece::size() const noexcept { return m_bytes.size(); }
std::size_t Piece::offset() const noexcept { return m_offset; }
std::vector<std::uint8_t> &Piece::vec() { return m_bytes; }
const std::vector<std::uint8_t> &Piece::vec() const { return m_bytes; }

} // namespace cactus
