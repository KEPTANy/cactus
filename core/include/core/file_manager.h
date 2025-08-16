#ifndef CACTUS_CORE_FILE_MANAGER_H
#define CACTUS_CORE_FILE_MANAGER_H

#include <string>
#include <vector>

namespace cactus {

struct File {
  std::string fname; // file name relative to save directory
  std::size_t size;
  std::size_t offset;
};

class FileManager {
public:
  FileManager() = default;

  void add_file(const std::string &fname, std::size_t size);

  std::size_t total_size() const;
  const std::vector<File> &files() const;

private:
  std::vector<File> m_files;
  std::size_t m_total_size{0};
};

} // namespace cactus

#endif
