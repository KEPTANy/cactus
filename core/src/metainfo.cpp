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
  if (pieces_cnt % SHA1::hash_size != 0) {
    return false;
  }
  pieces_cnt /= SHA1::hash_size;

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
  const auto &d = decoded.as_dict();
  const auto &d_info = d.at("info").as_dict();

  Metainfo res;

  res.m_encoded = encoded_metainfo;

  std::string info_str = d.at("info").encode();
  res.m_hash = SHA1::compute(info_str.data(), info_str.size());

  if (has(d, "created by", Entry::STR)) {
    res.m_creator = d.at("created by").as_str();
  }

  if (has(d, "comment", Entry::STR)) {
    res.m_comment = d.at("comment").as_str();
  }

  if (has(d, "creation date", Entry::INT)) {
    res.m_creation_time = d.at("creation date").as_int();
  }

  res.m_tracker = Tracker(d.at("announce").as_str());

  res.m_name = d_info.at("name").as_str();

  res.m_piece_size = d_info.at("piece length").as_int();

  const auto &pieces_str = d_info.at("pieces").as_str();
  std::size_t pieces_cnt = pieces_str.size() / SHA1::hash_size;
  res.m_pieces.resize(pieces_cnt);
  for (std::size_t i = 0; i < pieces_cnt; i++) {
    res.m_pieces[i] =
        SHA1::copy(reinterpret_cast<const std::uint8_t *>(pieces_str.data()) +
                   SHA1::hash_size * i);
  }

  if (has(d_info, "length", Entry::INT)) { // single file
    std::size_t size = d_info.at("length").as_int();
    res.m_storage.add_file(res.m_name, size);
  } else {
    const auto &files = d_info.at("files").as_list();
    for (const auto &f : files) {
      const auto &file = f.as_dict();

      std::size_t size = file.at("length").as_int();
      std::string path = res.m_name;

      const auto &path_list = file.at("path").as_list();
      for (const auto &e : path_list) {
        path += '/';
        path += e.as_str();
      }

      res.m_storage.add_file(path, size);
    }
  }

  return res;
}

const std::string &Metainfo::bencoded() const noexcept { return m_encoded; }

const std::string &Metainfo::name() const noexcept { return m_name; }
const std::string &Metainfo::creator() const noexcept { return m_creator; }
const std::string &Metainfo::comment() const noexcept { return m_comment; }
const std::time_t &Metainfo::creation_time() const noexcept {
  return m_creation_time;
}

const SHA1 &Metainfo::info_hash() const noexcept { return m_hash; }
const Tracker &Metainfo::tracker() const noexcept { return m_tracker; }
std::size_t Metainfo::piece_size() const noexcept { return m_piece_size; }
const std::vector<SHA1> &Metainfo::pieces() const noexcept { return m_pieces; }
const FileStorage &Metainfo::storage() const noexcept { return m_storage; }

} // namespace cactus
