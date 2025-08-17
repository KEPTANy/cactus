#include "core/file.h"

namespace cactus {

File::File(const std::string &rel_path, std::size_t size, std::size_t offset)
    : m_rel_path{rel_path}, m_size{size}, m_offset{offset} {}

void File::preallocate(const std::filesystem::path &save_path,
                       bool throw_if_exists) const {
  if (throw_if_exists && std::filesystem::exists(save_path / m_rel_path)) {
    throw std::runtime_error("File already exists");
  }

  try {
    std::filesystem::resize_file(save_path / m_rel_path, m_size);
  } catch (...) {
    throw std::runtime_error("Failed to resize file " +
                             (save_path / m_rel_path).string());
  }
}

const std::string &File::rel_path() const noexcept { return m_rel_path; }
std::size_t File::size() const noexcept { return m_size; }
std::size_t File::offset() const noexcept { return m_offset; }

} // namespace cactus
