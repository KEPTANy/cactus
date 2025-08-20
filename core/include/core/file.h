#ifndef CACTUS_CORE_FILE_H
#define CACTUS_CORE_FILE_H

#include <filesystem>
#include <string>

#include "core/piece.h"

namespace cactus {

class File {
public:
  // Path is relative to the save directory.
  // Offset is relative to the first byte of the first file of the torrent.
  File(const std::string &rel_path, std::size_t size, std::size_t offset);

  bool is_allocated(const std::filesystem::path &save_path) const noexcept;

  void preallocate(const std::filesystem::path &save_path,
                   bool throw_if_exists = false) const;

  void read_overlap(const std::filesystem::path &save_path, Piece &p) const;

  void write_overlap(const std::filesystem::path &save_path,
                     const Piece &p) const;

  const std::string &rel_path() const noexcept;
  std::size_t size() const noexcept;
  std::size_t offset() const noexcept;

private:
  std::string m_rel_path;
  std::size_t m_size;
  std::size_t m_offset;
};

} // namespace cactus

#endif
