#ifndef CACTUS_CORE_PIECE_MANAGER_H
#define CACTUS_CORE_PIECE_MANAGER_H

#include <vector>

#include "core/crypto.h"
#include "core/piece.h"

namespace cactus {

class PieceManager {
public:
  PieceManager() = default;

  PieceManager(std::vector<crypto::SHA1> &&hashes, std::size_t piece_size);

  const std::vector<crypto::SHA1> &hashes() const;
  std::size_t piece_size() const noexcept;
  std::size_t total_size() const noexcept;

  // Returns whether or not hashes matched and remembers if it needs to be
  // downloaded.
  bool verify_piece(const Piece &piece);

  void reset_downloaded_bitmask();

private:
  std::vector<crypto::SHA1> m_hashes;
  std::vector<bool> m_downloaded;
  std::size_t m_piece_size;
  std::size_t m_total_size;
};

} // namespace cactus

#endif
