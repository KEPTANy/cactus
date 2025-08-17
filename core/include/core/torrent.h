#ifndef CACTUS_CORE_TORRENT_H
#define CACTUS_CORE_TORRENT_H

#include <filesystem>

#include "core/crypto.h"
#include "core/metainfo.h"

namespace cactus {

class Torrent {
public:
  static Torrent create(const std::filesystem::path &save_path,
                        const std::filesystem::path &torrent_path);

private:
  std::filesystem::path m_save_path;

  Metainfo m_meta;
  crypto::SHA1 m_info_hash;

  TrackerManager m_tracker_mgr;
  PieceManager m_piece_mgr;
  FileManager m_file_mgr;
};

}

#endif
