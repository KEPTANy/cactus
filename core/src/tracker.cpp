#include "core/tracker.h"

namespace cactus {

Tracker::Tracker(const std::string &url) : m_url{url} {}

void Tracker::set_url(const std::string &url) { m_url = url; }

const std::string &Tracker::url() const noexcept { return m_url; }

} // namespace cactus
