#include "core/file_storage.h"

namespace cactus {

void FileStorage::add_file(const std::string &fname, std::size_t size) {
  std::size_t offset = 0;
  if (!m_files.empty()) {
    offset = m_files.back().offset + m_files.back().size;
  }

  m_files.push_back(File{fname, size, offset});
  m_total_size += size;
}

std::size_t FileStorage::total_size() const { return m_total_size; }
const std::vector<File> &FileStorage::files() const { return m_files; }

} // namespace cactus
