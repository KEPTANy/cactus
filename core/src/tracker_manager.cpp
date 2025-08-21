#include "core/tracker_manager.h"

namespace cactus {

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
