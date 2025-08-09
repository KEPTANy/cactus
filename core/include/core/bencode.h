#ifndef CORE_BENCODE_H
#define CORE_BENCODE_H

#include <cstdint>
#include <map>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

namespace bencode {

class Entry {
public:
  using Int = std::intmax_t;
  using Str = std::string;
  using List = std::vector<Entry>;
  using Dict = std::map<std::string, Entry>;

  enum Type { INT, STR, LIST, DICT };

  Entry(); // Default constructor initializes with 0
  Entry(Int);
  Entry(const Str &);
  Entry(const char *);
  Entry(const List &);
  Entry(const Dict &);
  Entry(Str &&);
  Entry(List &&);
  Entry(Dict &&);

  Entry &operator=(Int);
  Entry &operator=(const Str &);
  Entry &operator=(const char *);
  Entry &operator=(const List &);
  Entry &operator=(const Dict &);
  Entry &operator=(Str &&);
  Entry &operator=(List &&);
  Entry &operator=(Dict &&);

  Type type() const noexcept;
  bool is_int() const noexcept;
  bool is_str() const noexcept;
  bool is_list() const noexcept;
  bool is_dict() const noexcept;

  Int &as_int();
  Str &as_str();
  List &as_list();
  Dict &as_dict();

  const Int &as_int() const;
  const Str &as_str() const;
  const List &as_list() const;
  const Dict &as_dict() const;

  std::string encode() const;

private:
  std::variant<Int, Str, List, Dict> data;
};

Entry decode(std::string_view);

}; // namespace bencode

#endif
