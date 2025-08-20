#include "core/file_manager.h"

#include <algorithm>

namespace cactus {

void FileManager::add_file(const std::string &fname, std::size_t size) {
  std::size_t offset = 0;
  if (!m_files.empty()) {
    offset = m_files.back().offset() + m_files.back().size();
  }

  m_files.push_back(File(fname, size, offset));
  m_total_size += size;
}

std::size_t FileManager::total_size() const noexcept { return m_total_size; }
const std::vector<File> &FileManager::files() const noexcept { return m_files; }

void FileManager::preallocate(const std::filesystem::path &save_path,
                              bool throw_if_exists) const {
  for (const auto &f : m_files) {
    f.preallocate(save_path, throw_if_exists);
  }
}

void FileManager::read_piece(const std::filesystem::path &save_path,
                             Piece &p) const {
  std::size_t start{p.offset()};
  std::size_t end{p.size() + start};

  if (end > m_total_size) {
    throw std::runtime_error(
        "Piece end excedes the total size and therefore can't be read");
  }

  auto it = std::partition_point(
      m_files.begin(), m_files.end(),
      [=](const File &a) -> bool { return end <= a.offset(); });

  while (it != m_files.end() && start < it->offset() + it->size()) {
    it->read_overlap(save_path, p);
    it++;
  }
}

void FileManager::write_piece(const std::filesystem::path &save_path,
                              const Piece &p) const {
  std::size_t start{p.offset()};
  std::size_t end{p.size() + start};

  if (end > m_total_size) {
    throw std::runtime_error(
        "Piece end excedes the total size and therefore can't be read");
  }

  auto it = std::partition_point(
      m_files.begin(), m_files.end(),
      [=](const File &a) -> bool { return end <= a.offset(); });

  while (it != m_files.end() && start < it->offset() + it->size()) {
    it->write_overlap(save_path, p);
    it++;
  }
}

} // namespace cactus
