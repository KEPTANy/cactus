#ifndef CACTUS_CORE_FILE_H
#define CACTUS_CORE_FILE_H

#include <filesystem>
#include <string>

namespace cactus {

class File {
public:
  // Path is relative to the save directory.
  // Offset is relative to the first byte of the first file of the torrent.
  File(const std::string &rel_path, std::size_t size, std::size_t offset);

  void preallocate(const std::filesystem::path &save_path,
                   bool throw_if_exists = false) const;

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
