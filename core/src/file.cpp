#include "core/file.h"

#include <fstream>

namespace cactus {

File::File(const std::string &rel_path, std::size_t size, std::size_t offset)
    : m_rel_path{rel_path}, m_size{size}, m_offset{offset} {}

bool File::is_allocated(const std::filesystem::path &save_path) const noexcept {
  try {
    return std::filesystem::file_size(save_path / m_rel_path) == m_size;
  } catch (...) {
    return false;
  }
}

void File::preallocate(const std::filesystem::path &save_path,
                       bool throw_if_exists) const {

  auto full_path = save_path / m_rel_path;
  if (throw_if_exists && std::filesystem::exists(full_path)) {
    throw std::runtime_error("File already exists");
  }

  try {
    std::filesystem::create_directories(full_path.parent_path());
    if (!std::filesystem::exists(full_path)) {
      std::ofstream f(full_path);
      f.close();
    }
    std::filesystem::resize_file(full_path, m_size);
  } catch (...) {
    throw std::runtime_error("Failed to resize file " + (full_path).string());
  }
}

void File::read_overlap(const std::filesystem::path &save_path,
                        Piece &p) const {
  if (!is_allocated(save_path)) {
    preallocate(save_path);
  }

  std::size_t start{std::max(m_offset, p.offset())};
  std::size_t end{std::min(m_offset + m_size, p.offset() + p.size())};

  if (start > end) {
    return;
  }

  std::ifstream f(save_path / m_rel_path, std::ios::binary);
  f.seekg(start - m_offset);
  f.read(reinterpret_cast<char *>(p.vec().data() + start - p.offset()),
         end - start);
}

void File::write_overlap(const std::filesystem::path &save_path,
                         const Piece &p) const {
  if (!is_allocated(save_path)) {
    preallocate(save_path);
  }

  std::size_t start{std::max(m_offset, p.offset())};
  std::size_t end{std::min(m_offset + m_size, p.offset() + p.size())};

  if (start > end) {
    return;
  }

  std::ofstream f(save_path / m_rel_path, std::ios::binary);
  f.seekp(start - m_offset);
  f.write(reinterpret_cast<const char *>(p.vec().data() + start - p.offset()),
          end - start);
}

const std::string &File::rel_path() const noexcept { return m_rel_path; }
std::size_t File::size() const noexcept { return m_size; }
std::size_t File::offset() const noexcept { return m_offset; }

} // namespace cactus
