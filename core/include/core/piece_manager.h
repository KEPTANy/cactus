#ifndef CACTUS_CORE_PIECE_MANAGER_H
#define CACTUS_CORE_PIECE_MANAGER_H

#include <vector>

#include "core/sha1.h"

namespace cactus {

class PieceManager {
public:
  PieceManager(std::vector<SHA1> &&hashes, std::size_t piece_size);

  const std::vector<SHA1> &hashes() const;
  std::size_t piece_size() const;

private:
  std::vector<SHA1> m_hashes;
  std::size_t m_piece_size;
  std::size_t m_total_size;
};

} // namespace cactus

#endif
