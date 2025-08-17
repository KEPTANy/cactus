#ifndef CACTUS_CORE_PIECE_MANAGER_H
#define CACTUS_CORE_PIECE_MANAGER_H

#include <vector>

#include "core/crypto.h"

namespace cactus {

class PieceManager {
public:
  PieceManager(std::vector<crypto::SHA1> &&hashes, std::size_t piece_size);

  const std::vector<crypto::SHA1> &hashes() const;
  std::size_t piece_size() const;

private:
  std::vector<crypto::SHA1> m_hashes;
  std::size_t m_piece_size;
  std::size_t m_total_size;
};

} // namespace cactus

#endif
