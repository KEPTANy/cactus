#ifndef CACTUS_CORE_FILE_MANAGER_H
#define CACTUS_CORE_FILE_MANAGER_H

#include <string>
#include <vector>

#include "core/file.h"

namespace cactus {

class FileManager {
public:
  FileManager() = default;

  void add_file(const std::string &fname, std::size_t size);

  std::size_t total_size() const noexcept;
  const std::vector<File> &files() const noexcept;

  void preallocate(const std::filesystem::path &save_path,
                   bool throw_if_exists = false) const;

  void read_piece(const std::filesystem::path &save_path, Piece &p) const;

  void write_piece(const std::filesystem::path &save_path,
                   const Piece &p) const;

private:
  std::vector<File> m_files;
  std::size_t m_total_size{0};
};

} // namespace cactus

#endif
