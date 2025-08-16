#ifndef CACTUS_CORE_METAINFO_H
#define CACTUS_CORE_METAINFO_H

#include <ctime>
#include <filesystem>
#include <string>
#include <string_view>
#include <vector>

#include "core/bencode.h"
#include "core/file_manager.h"
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

  SHA1 info_hash() const;

  std::string creator() const;
  std::string comment() const;
  std::time_t creation_time() const;

  std::string name() const;
  Tracker tracker() const;
  std::size_t piece_size() const;
  std::vector<SHA1> pieces() const;
  FileManager file_manager() const;

private:
  std::string m_encoded;
  bencode::Entry::Dict m_decoded;
};

} // namespace cactus

#endif
