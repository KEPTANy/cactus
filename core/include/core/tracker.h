#ifndef CACTUS_CORE_TRACKER_H
#define CACTUS_CORE_TRACKER_H

#include <string>

#include "core/crypto.h"
#include "core/peer.h"
#include "core/peer_id.h"

namespace cactus {

class Tracker {
public:
  enum Event { STARTED, COMPLETED, STOPPED };

  Tracker() = default;
  Tracker(const std::string &url);

  void set_url(const std::string &url);
  const std::string &url() const noexcept;

  std::vector<Peer> announce(const crypto::SHA1 &info_hash,
                             const PeerID &peer_id, std::uint16_t port,
                             std::size_t uploaded, std::size_t downloaded,
                             std::size_t left, bool compact, Event event);

private:
  std::string m_url;
};

} // namespace cactus

#endif
