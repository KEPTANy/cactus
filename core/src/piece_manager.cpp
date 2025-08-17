#include "core/piece_manager.h"

namespace cactus {

PieceManager::PieceManager(std::vector<crypto::SHA1> &&hashes,
                           std::size_t piece_size)
    : m_hashes{std::move(hashes)}, m_piece_size{piece_size} {}

const std::vector<crypto::SHA1> &PieceManager::hashes() const {
  return m_hashes;
}

std::size_t PieceManager::piece_size() const { return m_piece_size; }

} // namespace cactus
