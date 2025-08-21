#ifndef CACTUS_CORE_TRACKER_H
#define CACTUS_CORE_TRACKER_H

#include <string>

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

} // namespace cactus

#endif
