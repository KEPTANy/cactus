#ifndef CACTUS_CORE_METAINFO_H
#define CACTUS_CORE_METAINFO_H

#include <ctime>
#include <filesystem>
#include <string>
#include <string_view>
#include <vector>

#include "core/file_storage.h"
#include "core/sha1.h"
#include "core/tracker.h"

namespace cactus {

class Metainfo {
public:
  static Metainfo parse(std::string_view encoded_metainfo,
                        bool validate = true);

  static Metainfo from_file(const std::filesystem::path &fname,
                            bool validate = true);

  const std::string &bencoded() const noexcept;

  const std::string &creator() const noexcept;
  const std::string &comment() const noexcept;
  const std::time_t &creation_time() const noexcept;

  const SHA1 &info_hash() const noexcept;
  const std::string &name() const noexcept;
  const Tracker &tracker() const noexcept;
  std::size_t piece_size() const noexcept;
  const std::vector<SHA1> &pieces() const noexcept;
  const FileStorage &storage() const noexcept;

private:
  std::string m_encoded;
  SHA1 m_hash;

  std::string m_creator{""};
  std::string m_comment{""};
  std::time_t m_creation_time{0};

  Tracker m_tracker;
  std::string m_name;
  std::size_t m_piece_size;
  std::vector<SHA1> m_pieces;
  FileStorage m_storage;
};

} // namespace cactus

#endif
