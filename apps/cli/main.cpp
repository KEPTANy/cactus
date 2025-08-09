#include <cstdlib>
#include <iostream>

#include "core/bencode.h"

using namespace bencode;

void print(Entry &val) {
  switch (val.type()) {
  case Entry::INT: std::cout << val.as_int(); break;
  case Entry::STR: std::cout << '\"' << val.as_str() << '\"'; break;
  case Entry::LIST:
    std::cout << "[\n";
    for (auto &e : val.as_list()) {
      print(e);
      std::cout << ",\n";
    }
    std::cout << ']';
    break;
  case Entry::DICT:
    std::cout << "{\n";
    for (auto &[k, v] : val.as_dict()) {
      std::cout << '\"' << k << "\": ";
      print(v);
      std::cout << ",\n";
    }
    std::cout << '}';
    break;
  }
}

int main() {
  std::string benc;
  std::cin >> benc;

  Entry e{decode(benc)};
  print(e);
  return EXIT_SUCCESS;
}
