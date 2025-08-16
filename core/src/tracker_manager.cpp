#include "core/tracker_manager.h"

namespace cactus {

Tracker::Tracker(const std::string &url) : m_url{url} {}

void Tracker::set_url(const std::string &url) { m_url = url; }

const std::string &Tracker::url() const noexcept { return m_url; }

void TrackerManager::add_tracker(std::string url, std::size_t tier) {
  if (m_trackers.size() <= tier) {
    m_trackers.resize(tier + 1);
  }
  m_trackers[tier].emplace_back(url);
}

const std::vector<std::vector<Tracker>> &
TrackerManager::trackers() const noexcept {
  return m_trackers;
}

} // namespace cactus
