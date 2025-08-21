#ifndef CACTUS_CORE_PEER_ID_H
#define CACTUS_CORE_PEER_ID_H

#include <array>
#include <cstdint>
#include <random>
#include <string_view>

namespace cactus {

class PeerID {
public:
  // Generates a new, random peer_id
  PeerID();
  
  PeerID(std::string_view str);

  constexpr const std::uint8_t *data() const noexcept;
  constexpr std::size_t size() const noexcept;

private:
  std::array<std::uint8_t, 20> m_id;

  inline static std::random_device m_rd;
};

}

#endif
