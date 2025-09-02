#ifndef CACTUS_CORE_PEER_H
#define CACTUS_CORE_PEER_H

#include <asio.hpp>

namespace cactus {

class Peer {
public:
  Peer(asio::ip::address_v4 ip, std::uint16_t port);

  asio::ip::address_v4 ip() const noexcept;
  std::uint16_t port() const noexcept;

private:
  asio::ip::address_v4 m_ip;
  std::uint16_t m_port;
};

}

#endif
