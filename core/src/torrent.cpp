#include "core/torrent.h"

#include <filesystem>

namespace cactus {

Torrent Torrent::create(const std::filesystem::path &save_path,
                        const std::filesystem::path &torrent_path) {
  Torrent res;
  res.m_meta = Metainfo::from_file(torrent_path);

  if (std::filesystem::exists(save_path / res.m_meta.name())) {
    throw std::runtime_error(
        "Couldn't use this save path because there's a name colision");
  }

  res.m_save_path = save_path;

  res.m_info_hash = res.m_meta.info_hash();
  res.m_tracker_mgr = res.m_meta.tracker_manager();
  res.m_piece_mgr = res.m_meta.piece_manager();
  res.m_file_mgr = res.m_meta.file_manager();

  return res;
}

} // namespace cactus
