#include "core/file_manager.h"

namespace cactus {

void FileManager::add_file(const std::string &fname, std::size_t size) {
  std::size_t offset = 0;
  if (!m_files.empty()) {
    offset = m_files.back().offset + m_files.back().size;
  }

  m_files.push_back(File{fname, size, offset});
  m_total_size += size;
}

std::size_t FileManager::total_size() const { return m_total_size; }
const std::vector<File> &FileManager::files() const { return m_files; }

} // namespace cactus
