#include "core/torrent.h"

#include <algorithm>
#include <filesystem>

namespace cactus {

Torrent Torrent::create(const std::filesystem::path &save_path,
                        const std::filesystem::path &torrent_path,
                        bool throw_on_name_collision) {
  if (!std::filesystem::is_directory(save_path)) {
    throw std::runtime_error("Provided save path must be a directory");
  }

  if (!std::filesystem::is_regular_file(torrent_path)) {
    throw std::runtime_error("Provided torrent path must be a file");
  }

  Torrent res;
  res.m_meta = Metainfo::from_file(torrent_path);

  if (throw_on_name_collision && std::filesystem::exists(save_path / res.m_meta.name())) {
    throw std::runtime_error(
        "Couldn't use this save path because there's a name collision");
  }

  res.m_save_path = save_path;

  res.m_info_hash = res.m_meta.info_hash();
  res.m_tracker_mgr = res.m_meta.tracker_manager();
  res.m_piece_mgr = res.m_meta.piece_manager();
  res.m_file_mgr = res.m_meta.file_manager();

  return res;
}

bool Torrent::reverify() {
  m_piece_mgr.reset_downloaded_bitmask();

  std::size_t cnt = m_piece_mgr.hashes().size();
  std::size_t total_size = m_piece_mgr.total_size();
  std::size_t piece_size = m_piece_mgr.piece_size();

  bool res = true;
  for (std::size_t i = 0; i < cnt; i++) {
    std::size_t size = piece_size;
    if (i == cnt - 1) {
      size = total_size % piece_size;
    }

    Piece piece(size, piece_size * i, i);
    try {
      m_file_mgr.read_piece(m_save_path, piece);
    } catch (...) {
      throw std::runtime_error("Failed to read a piece #" + std::to_string(i));
    }

    res |= m_piece_mgr.verify_piece(piece);
  }

  return res;
}

} // namespace cactus
