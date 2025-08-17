#ifndef CACTUS_CORE_METAINFO_H
#define CACTUS_CORE_METAINFO_H

#include <ctime>
#include <filesystem>
#include <string>
#include <string_view>

#include "core/bencode.h"
#include "core/crypto.h"
#include "core/file_manager.h"
#include "core/piece_manager.h"
#include "core/tracker_manager.h"

namespace cactus {

class Metainfo {
public:
  static Metainfo parse(std::string_view encoded_metainfo,
                        bool validate = true);

  static Metainfo from_file(const std::filesystem::path &fname,
                            bool validate = true);

  const std::string &bencoded() const noexcept;

  crypto::SHA1 info_hash() const;

  std::string creator() const;
  std::string comment() const;
  std::time_t creation_time() const;

  TrackerManager tracker_manager() const;
  PieceManager piece_manager() const;
  FileManager file_manager() const;

private:
  std::string m_encoded;
  bencode::Entry::Dict m_decoded;
};

} // namespace cactus

#endif
