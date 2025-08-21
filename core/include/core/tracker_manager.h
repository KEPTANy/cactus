#ifndef CACTUS_CORE_TRACKER_MANAGER_H
#define CACTUS_CORE_TRACKER_MANAGER_H

#include <vector>

#include "core/tracker.h"

namespace cactus {

class TrackerManager {
public:
  TrackerManager() = default;

  void add_tracker(std::string url, std::size_t tier = 0);

  const std::vector<std::vector<Tracker>> &trackers() const noexcept;

private:
  // Might want to consider std::list<> as a container for a tier
  // (to make moving a tracker to the begining O(1)) but there's not that many
  // elements so who cares
  std::vector<std::vector<Tracker>> m_trackers;
};

} // namespace cactus

#endif
