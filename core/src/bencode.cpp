#include "core/bencode.h"

#include <charconv>
#include <stdexcept>
#include <variant>

namespace cactus {

namespace bencode {

Entry::Entry() : data{0} {}
Entry::Entry(Int x) : data{x} {}
Entry::Entry(const Str &x) : data{x} {}
Entry::Entry(const char *x) : data{std::string(x)} {}
Entry::Entry(const List &x) : data{x} {}
Entry::Entry(const Dict &x) : data{x} {}
Entry::Entry(Str &&x) : data{std::move(x)} {}
Entry::Entry(List &&x) : data{std::move(x)} {}
Entry::Entry(Dict &&x) : data{std::move(x)} {}

Entry &Entry::operator=(Int v) {
  data = v;
  return *this;
}

Entry &Entry::operator=(const Str &v) {
  data = v;
  return *this;
}

Entry &Entry::operator=(const char *v) {
  data = std::string(v);
  return *this;
}

Entry &Entry::operator=(const List &v) {
  data = v;
  return *this;
}

Entry &Entry::operator=(const Dict &v) {
  data = v;
  return *this;
}

Entry &Entry::operator=(Str &&v) {
  data = std::move(v);
  return *this;
}

Entry &Entry::operator=(List &&v) {
  data = std::move(v);
  return *this;
}

Entry &Entry::operator=(Dict &&v) {
  data = std::move(v);
  return *this;
}

Entry::Type Entry::type() const noexcept {
  return static_cast<Entry::Type>(data.index());
}

bool Entry::is_int() const noexcept {
  return std::holds_alternative<Entry::Int>(data);
}

bool Entry::is_str() const noexcept {
  return std::holds_alternative<Entry::Str>(data);
}

bool Entry::is_list() const noexcept {
  return std::holds_alternative<Entry::List>(data);
}

bool Entry::is_dict() const noexcept {
  return std::holds_alternative<Entry::Dict>(data);
}

Entry::Int &Entry::as_int() {
  if (!is_int()) {
    throw std::bad_variant_access{};
  }
  return std::get<Int>(data);
}

Entry::Str &Entry::as_str() {
  if (!is_str()) {
    throw std::bad_variant_access{};
  }
  return std::get<Str>(data);
}

Entry::List &Entry::as_list() {
  if (!is_list()) {
    throw std::bad_variant_access{};
  }
  return std::get<List>(data);
}

Entry::Dict &Entry::as_dict() {
  if (!is_dict()) {
    throw std::bad_variant_access{};
  }
  return std::get<Dict>(data);
}

const Entry::Int &Entry::as_int() const {
  if (!is_int()) {
    throw std::bad_variant_access{};
  }
  return std::get<Int>(data);
}

const Entry::Str &Entry::as_str() const {
  if (!is_str()) {
    throw std::bad_variant_access{};
  }
  return std::get<Str>(data);
}

const Entry::List &Entry::as_list() const {
  if (!is_list()) {
    throw std::bad_variant_access{};
  }
  return std::get<List>(data);
}

const Entry::Dict &Entry::as_dict() const {
  if (!is_dict()) {
    throw std::bad_variant_access{};
  }
  return std::get<Dict>(data);
}

std::string Entry::encode() const {
  struct EncodeVisitor {
    std::string operator()(Int val) const {
      return "i" + std::to_string(val) + "e";
    }

    std::string operator()(const Str &str) const {
      return std::to_string(str.size()) + ":" + str;
    }

    std::string operator()(const List &list) const {
      std::string res{"l"};
      for (const auto &e : list) {
        res += std::visit(*this, e.data);
      }
      return res + "e";
    }

    std::string operator()(const Dict &dict) const {
      std::string res{"d"};
      for (const auto &[key, val] : dict) {
        res += (*this)(key);
        res += std::visit(*this, val.data);
      }
      return res + "e";
    }
  };

  return std::visit(EncodeVisitor{}, data);
}

bool Entry::operator==(const Entry &other) const { return data == other.data; }

bool Entry::operator!=(const Entry &other) const { return !(*this == other); }

static void consume(std::string_view &s, char c, const char *err) {
  if (s.empty() || s[0] != c) {
    throw std::runtime_error(err);
  }

  s.remove_prefix(1);
}

Entry decode(std::string_view str) {
  struct Decoder {
    std::string_view str;

    Entry decode() {
      if (str.empty()) {
        throw std::runtime_error("Unexpected end of bencode string");
      }

      switch (str[0]) {
      case 'i':
        return decode_int();
      case 'l':
        return decode_list();
      case 'd':
        return decode_dict();
      default:
        if (std::isdigit(str[0])) {
          return decode_str();
        }
        throw std::runtime_error("Unknown bencode type character");
      }
    }

    Entry::Int decode_int() {
      consume(str, 'i', "Bad integer type character");

      auto end_pos{str.find('e')};
      if (end_pos == str.npos) {
        throw std::runtime_error("Unterminated integer");
      }

      Entry::Int res;
      auto [ptr, ec] = std::from_chars(str.data(), str.data() + end_pos, res);

      if (ec != std::errc() || ptr != str.data() + end_pos) {
        throw std::runtime_error("Invalid integer format");
      }

      if (str[0] == '0' && end_pos > 1) {
        throw std::runtime_error("Leading zeros are not allowed");
      }

      if (str.starts_with("-0")) {
        throw std::runtime_error("Negative zero not allowed");
      }

      str.remove_prefix(end_pos + 1);
      return res;
    }

    Entry::Str decode_str() {
      size_t colon_pos = str.find(':');
      if (colon_pos == std::string_view::npos) {
        throw std::runtime_error("Missing colon in string");
      }

      size_t length = 0;
      auto [ptr, ec] =
          std::from_chars(str.data(), str.data() + colon_pos, length);

      if (ec != std::errc() || ptr != str.data() + colon_pos) {
        throw std::runtime_error("Invalid string length");
      }

      if (str[0] == '0' && colon_pos > 1) {
        throw std::runtime_error(
            "Leading zeros in string size are not allowed");
      }

      if (str.size() < colon_pos + 1 + length) {
        throw std::runtime_error("String length exceeds available str");
      }

      Entry::Str res(str.substr(colon_pos + 1, length));
      str.remove_prefix(colon_pos + 1 + length);
      return res;
    }

    Entry::List decode_list() {
      consume(str, 'l', "Bad list type character");

      Entry::List res;
      while (!str.empty() && str[0] != 'e') {
        res.push_back(decode());
      }

      consume(str, 'e', "Bad list end character or unterminated list");
      return res;
    }

    Entry::Dict decode_dict() {
      consume(str, 'd', "Bad dictionary type character");

      Entry::Dict res;
      Entry::Str prev_key;
      while (!str.empty() && str[0] != 'e') {
        auto key{decode_str()};

        if (!res.empty() && key <= prev_key) {
          throw std::runtime_error("Dictionary keys not sorted");
        }

        if (res.find(key) != res.end()) {
          throw std::runtime_error("Dublicate dictionary keys");
        }

        auto val{decode()};
        res[key] = val;

        prev_key = key;
      }

      consume(str, 'e',
              "Bad dictionary end character or unterminated dictionary");
      return res;
    }
  };

  Decoder decoder{str};
  Entry res{decoder.decode()};

  if (!decoder.str.empty()) {
    throw std::runtime_error("Bencode string isn't empty after parsing");
  }

  return res;
}

}; // namespace bencode

}; // namespace cactus
