#include "core/piece_manager.h"

namespace cactus {

PieceManager::PieceManager(std::vector<crypto::SHA1> &&hashes,
                           std::size_t piece_size)
    : m_hashes(std::move(hashes)), m_piece_size{piece_size},
      m_downloaded(m_hashes.size(), false) {}

const std::vector<crypto::SHA1> &PieceManager::hashes() const {
  return m_hashes;
}

std::size_t PieceManager::piece_size() const noexcept { return m_piece_size; }
std::size_t PieceManager::total_size() const noexcept { return m_total_size; }

bool PieceManager::verify_piece(const Piece &piece) {
  if (m_hashes.size() <= piece.index()) {
    return false;
  }

  // Better check piece sizes too but hash collisions are unlikely so let's
  // forget about that for now
  if (piece.sha1() != m_hashes[piece.index()]) {
    return false;
  }

  m_downloaded[piece.index()] = true;
  return true;
}

void PieceManager::reset_downloaded_bitmask() {
  m_downloaded.assign(m_downloaded.size(), false);
}

} // namespace cactus
