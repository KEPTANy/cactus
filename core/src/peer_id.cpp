#include "core/peer_id.h"

#include <cstring>
#include <stdexcept>

namespace cactus {

PeerID::PeerID() {
  static std::uniform_int_distribution<std::uint16_t> dist(0, 255);
  for (auto &byte : m_id) {
    byte = static_cast<std::uint8_t>(dist(m_rd));
  }
}

PeerID::PeerID(std::string_view str) {
  if (str.size() != size()) {
    throw std::runtime_error("Bad string size in PeerID initializer");
  }

  std::memcpy(m_id.data(), str.data(), size());
}

const std::uint8_t *PeerID::begin() const noexcept { return m_id.begin(); }
const std::uint8_t *PeerID::end() const noexcept { return m_id.end(); }

constexpr std::size_t PeerID::size() const noexcept { return m_id.size(); }

}
