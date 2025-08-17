#include "core/metainfo.h"

#include <filesystem>
#include <fstream>
#include <stdexcept>

#include "core/bencode.h"

namespace cactus {

using namespace bencode;

static bool has(const Entry::Dict &dict, const char *key, Entry::Type type) {
  if (dict.find(key) == dict.end()) {
    return false;
  }

  return dict.at(key).type() == type;
}

static bool is_valid_metainfo(const Entry &decoded_metainfo) {
  if (!decoded_metainfo.is_dict()) {
    return false;
  }

  const auto &d = decoded_metainfo.as_dict();
  if (!has(d, "announce", Entry::STR) || !has(d, "info", Entry::DICT)) {
    return false;
  }

  const auto &info = d.at("info").as_dict();
  if (!has(info, "name", Entry::STR) ||
      !has(info, "piece length", Entry::INT) ||
      !has(info, "pieces", Entry::STR)) {
    return false;
  }

  std::size_t piece_size = info.at("piece length").as_int();
  std::size_t pieces_cnt = info.at("pieces").as_str().size();
  if (pieces_cnt % crypto::SHA1::hash_size != 0) {
    return false;
  }
  pieces_cnt /= crypto::SHA1::hash_size;

  std::size_t total_size = 0;
  if (has(info, "length", Entry::INT)) {
    if (has(info, "files", Entry::LIST)) {
      return false;
    }

    total_size = info.at("length").as_int();
  } else if (has(info, "files", Entry::LIST)) {
    for (const auto &f : info.at("files").as_list()) {
      const auto &file = f.as_dict();

      if (!has(file, "length", Entry::INT) || !has(file, "path", Entry::LIST)) {
        return false;
      }

      total_size += file.at("length").as_int();

      for (const auto &e : file.at("path").as_list()) {
        if (!e.is_str()) {
          return false;
        }
      }
    }
  } else {
    return false;
  }

  if (pieces_cnt != ((total_size + piece_size - 1) / piece_size)) {
    return false;
  }

  return true;
}

Metainfo Metainfo::from_file(const std::filesystem::path &fname,
                             bool validate) {
  std::string meta(std::filesystem::file_size(fname), '\0');
  std::ifstream file(fname, std::ios::binary | std::ios::in);

  if (file.fail()) {
    throw std::runtime_error("Failed to open file " + fname.string());
  }

  file.read(meta.data(), meta.size());
  if (file.tellg() != meta.size()) {
    throw std::runtime_error("Failed to read contents of " + fname.string());
  }

  return parse(meta, validate);
}

Metainfo Metainfo::parse(std::string_view encoded_metainfo, bool validate) {
  const auto decoded{decode(encoded_metainfo)};
  if (validate && !is_valid_metainfo(decoded)) {
    throw std::runtime_error("Provided metainfo isn't valid");
  }

  Metainfo res;
  res.m_encoded = encoded_metainfo;
  res.m_decoded = decoded.as_dict();

  return res;
}

const std::string &Metainfo::bencoded() const noexcept { return m_encoded; }

crypto::SHA1 Metainfo::info_hash() const {
  const std::string &info_str = m_decoded.at("info").encode();
  return crypto::SHA1::compute(info_str.data(), info_str.size());
}

std::string Metainfo::creator() const {
  if (has(m_decoded, "created by", Entry::STR)) {
    return m_decoded.at("created by").as_str();
  }
  return "";
}

std::string Metainfo::comment() const {
  if (has(m_decoded, "comment", Entry::STR)) {
    return m_decoded.at("comment").as_str();
  }
  return "";
}

std::time_t Metainfo::creation_time() const {
  if (has(m_decoded, "creation date", Entry::INT)) {
    return m_decoded.at("creation date").as_int();
  }
  return 0;
}

TrackerManager Metainfo::tracker_manager() const {
  // TODO: support announce-list
  TrackerManager res;
  res.add_tracker(m_decoded.at("announce").as_str());
  return res;
}

PieceManager Metainfo::piece_manager() const {
  std::size_t piece_size =
      m_decoded.at("info").as_dict().at("piece length").as_int();

  const auto &pieces_str = m_decoded.at("info").as_dict().at("pieces").as_str();
  std::size_t pieces_cnt = pieces_str.size() / crypto::SHA1::hash_size;
  std::vector<crypto::SHA1> pieces(pieces_cnt);
  for (std::size_t i = 0; i < pieces_cnt; i++) {
    pieces[i] = crypto::SHA1::copy(
        reinterpret_cast<const std::uint8_t *>(pieces_str.data()) +
        crypto::SHA1::hash_size * i);
  }

  return PieceManager(std::move(pieces), piece_size);
}

FileManager Metainfo::file_manager() const {
  const auto &d_info = m_decoded.at("info").as_dict();
  std::string name = m_decoded.at("info").as_dict().at("name").as_str();

  FileManager res;
  if (has(d_info, "length", Entry::INT)) { // single file
    std::size_t size = d_info.at("length").as_int();
    res.add_file(name, size);
  } else {
    const auto &files = d_info.at("files").as_list();
    for (const auto &f : files) {
      const auto &file = f.as_dict();

      std::size_t size = file.at("length").as_int();
      std::string path = name;

      const auto &path_list = file.at("path").as_list();
      for (const auto &e : path_list) {
        path += '/';
        path += e.as_str();
      }

      res.add_file(path, size);
    }
  }

  return res;
}

} // namespace cactus
