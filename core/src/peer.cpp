#include "core/peer.h"

namespace cactus {

Peer::Peer(asio::ip::address_v4 ip, std::uint16_t port)
    : m_ip{ip}, m_port{port} {}

asio::ip::address_v4 Peer::ip() const noexcept { return m_ip; }
std::uint16_t Peer::port() const noexcept { return m_port; }

} // namespace cactus
