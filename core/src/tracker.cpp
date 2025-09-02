#include "core/tracker.h"

#include "core/bencode.h"
#include <cpr/cpr.h>

namespace cactus {

static const char *event_to_str(Tracker::Event e) {
  switch (e) {
  case Tracker::STARTED:
    return "started";
    break;
  case Tracker::COMPLETED:
    return "completed";
    break;
  case Tracker::STOPPED:
    return "stopped";
    break;
  default:
    throw std::runtime_error("Unexpected event value");
  }
}

Tracker::Tracker(const std::string &url) : m_url{url} {}

void Tracker::set_url(const std::string &url) { m_url = url; }

const std::string &Tracker::url() const noexcept { return m_url; }

static std::vector<Peer> decode_peers_compact(const bencode::Entry::Str &peers) {
  if (peers.size() % 6 != 0) {
    throw std::runtime_error("Peers string size isn't valid");
  }

  std::vector<Peer> res;
  for (std::size_t i = 0; i < peers.size(); i += 6) {
    std::uint32_t ipv4;
    std::uint16_t port;

    ipv4 = static_cast<std::uint32_t>(peers[i]);
    ipv4 += static_cast<std::uint32_t>(peers[i + 1]) << 8;
    ipv4 += static_cast<std::uint32_t>(peers[i + 2]) << 16;
    ipv4 += static_cast<std::uint32_t>(peers[i + 3]) << 24;

    port = static_cast<std::uint16_t>(peers[i + 4]);
    port += static_cast<std::uint16_t>(peers[i + 5]) << 8;

    res.push_back(Peer(asio::ip::address_v4(ipv4), port));
  }

  return res;
}

std::vector<Peer> Tracker::announce(const crypto::SHA1 &info_hash,
                                    const PeerID &peer_id, std::uint16_t port,
                                    std::size_t uploaded,
                                    std::size_t downloaded, std::size_t left,
                                    bool compact, Event event) {
  auto response = cpr::Get(
      cpr::Url{m_url},
      cpr::Parameters{
          {"info_hash", std::string(info_hash.begin(), info_hash.end())},
          {"peer_id", std::string(peer_id.begin(), peer_id.end())},
          {"port", std::to_string(port)},
          {"uploaded", std::to_string(uploaded)},
          {"downloaded", std::to_string(downloaded)},
          {"left", std::to_string(left)},
          {"compact", compact ? "1" : "0"},
          {"event", event_to_str(event)}});

  if (response.status_code != 200) {
    throw std::runtime_error(
        "Failed to retrieve peers from a tracker, HTTP status code: " +
        std::to_string(response.status_code));
  }

  bencode::Entry::Dict decoded_response;
  try {
    decoded_response = bencode::decode(response.text).as_dict();
  } catch (...) {
    throw std::runtime_error("Failed to decode tracker's response");
  }

  if (decoded_response.find("failure reason") != decoded_response.end()) {
    throw std::runtime_error("Announce failure: " +
                             decoded_response.at("failure reason").as_str());
  }

  if (decoded_response.find("peers") == decoded_response.end()) {
    throw std::runtime_error("Couldn't extract peers from a tracker response");
  }

  auto &peers = decoded_response.at("peers");
  if (peers.is_str()) {
    return decode_peers_compact(peers.as_str());
  } else if (peers.is_list()) {
    throw std::runtime_error("Non compact peer lists are not supported, sorry");
  }

  throw std::runtime_error("Peers isn't a string nor a list");
}

} // namespace cactus
