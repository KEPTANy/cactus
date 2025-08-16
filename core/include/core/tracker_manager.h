#ifndef CACTUS_CORE_TRACKER_MANAGER_H
#define CACTUS_CORE_TRACKER_MANAGER_H

#include <string>
#include <vector>

namespace cactus {

class Tracker {
public:
  Tracker() = default;
  Tracker(const std::string &url);

  void set_url(const std::string &url);
  const std::string &url() const noexcept;

private:
  std::string m_url;
};

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
