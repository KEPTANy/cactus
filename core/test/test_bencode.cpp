#include "gtest/gtest.h"

#include "core/bencode.h"

using namespace cactus::bencode;

std::string err;

TEST(Bencode, ConstructInt) {
  err = "Default constructor should initialize with 0";
  Entry int1;
  EXPECT_TRUE(int1.is_int() && int1.as_int() == 0) << err;
  EXPECT_EQ(int1, Entry(static_cast<Entry::Int>(0))) << err;
  EXPECT_EQ(int1.type(), Entry::INT) << err;
  EXPECT_ANY_THROW(int1.as_str()) << err;
  EXPECT_ANY_THROW(int1.as_list()) << err;
  EXPECT_ANY_THROW(int1.as_dict()) << err;
  EXPECT_EQ(int1.encode(), "i0e") << err;
  EXPECT_EQ(int1, decode("i0e")) << err;

  err = "Bad (Int) constructor";
  Entry int2(42);
  EXPECT_TRUE(int2.is_int()) << err;
  EXPECT_EQ(int2.as_int(), 42) << err;
  EXPECT_EQ(int2.type(), Entry::INT) << err;
  EXPECT_ANY_THROW(int2.as_str()) << err;
  EXPECT_ANY_THROW(int2.as_list()) << err;
  EXPECT_ANY_THROW(int2.as_dict()) << err;
  EXPECT_EQ(int2.encode(), "i42e") << err;
  EXPECT_EQ(int2, decode("i42e")) << err;

  err = "Bad (Int) operator=";
  int2 = -123;
  EXPECT_TRUE(int2.is_int()) << err;
  EXPECT_EQ(int2.as_int(), -123) << err;
  EXPECT_EQ(int2.type(), Entry::INT) << err;
  EXPECT_ANY_THROW(int2.as_str()) << err;
  EXPECT_ANY_THROW(int2.as_list()) << err;
  EXPECT_ANY_THROW(int2.as_dict()) << err;
  EXPECT_EQ(int2.encode(), "i-123e") << err;
  EXPECT_EQ(int2, decode("i-123e")) << err;
}

TEST(Bencode, ConstructString) {
  err = "Bad (const char *) constructor";
  Entry str1("test string lol");
  EXPECT_TRUE(str1.is_str()) << err;
  EXPECT_EQ(str1.as_str(), "test string lol") << err;
  EXPECT_EQ(str1.type(), Entry::STR) << err;
  EXPECT_ANY_THROW(str1.as_int()) << err;
  EXPECT_ANY_THROW(str1.as_list()) << err;
  EXPECT_ANY_THROW(str1.as_dict()) << err;
  EXPECT_EQ(str1.encode(), "15:test string lol") << err;
  EXPECT_EQ(str1, decode("15:test string lol")) << err;

  err = "Bad (const char *) operator=";
  str1 = "i could've come up with something better huh";
  EXPECT_TRUE(str1.is_str()) << err;
  EXPECT_EQ(str1.as_str(), "i could've come up with something better huh") << err;
  EXPECT_EQ(str1.type(), Entry::STR) << err;
  EXPECT_ANY_THROW(str1.as_int()) << err;
  EXPECT_ANY_THROW(str1.as_list()) << err;
  EXPECT_ANY_THROW(str1.as_dict()) << err;
  EXPECT_EQ(str1.encode(), "44:i could've come up with something better huh")
      << err;
  EXPECT_EQ(str1, decode("44:i could've come up with something better huh")) << err;

  err = "Bad (Str) constructor";
  std::string yet_another_str("std::string thingy");
  Entry str2(yet_another_str);
  EXPECT_TRUE(str2.is_str()) << err;
  EXPECT_EQ(str2.as_str(), "std::string thingy") << err;
  EXPECT_EQ(str2.type(), Entry::STR) << err;
  EXPECT_ANY_THROW(str2.as_int()) << err;
  EXPECT_ANY_THROW(str2.as_list()) << err;
  EXPECT_ANY_THROW(str2.as_dict()) << err;
  EXPECT_EQ(str2.encode(), "18:std::string thingy") << err;
  EXPECT_EQ(str2, decode("18:std::string thingy")) << err;

  err = "Bad (Str) operator=";
  std::string copy_me_please("i'm bouta be copied nooooooooo");
  str2 = copy_me_please;
  EXPECT_TRUE(str2.is_str()) << err;
  EXPECT_EQ(str2.as_str(), "i'm bouta be copied nooooooooo") << err;
  EXPECT_EQ(str2.type(), Entry::STR) << err;
  EXPECT_ANY_THROW(str2.as_int()) << err;
  EXPECT_ANY_THROW(str2.as_list()) << err;
  EXPECT_ANY_THROW(str2.as_dict()) << err;
  EXPECT_EQ(str2.encode(), "30:i'm bouta be copied nooooooooo") << err;
  EXPECT_EQ(str2, decode("30:i'm bouta be copied nooooooooo")) << err;

  std::string all_bytes(256, '\0');
  for (int i = 0; i < 256; i++) {
    all_bytes[i] = i;
  }
  std::string all_bytes_copy = all_bytes;

  err = "Bad (Str &&) constructor";
  Entry str3(std::move(all_bytes));
  EXPECT_TRUE(str3.is_str()) << err;
  EXPECT_EQ(str3.as_str(), all_bytes_copy) << err;
  EXPECT_EQ(str3.type(), Entry::STR) << err;
  EXPECT_ANY_THROW(str3.as_int()) << err;
  EXPECT_ANY_THROW(str3.as_list()) << err;
  EXPECT_ANY_THROW(str3.as_dict()) << err;
  EXPECT_EQ(str3.encode(), "256:" + all_bytes_copy) << err;
  EXPECT_EQ(str3, decode("256:" + all_bytes_copy)) << err;

  err = "Bad (Str &&) operator=";
  str3 = std::move(std::string(""));
  EXPECT_TRUE(str3.is_str()) << err;
  EXPECT_EQ(str3.as_str(), "") << err;
  EXPECT_EQ(str3.type(), Entry::STR) << err;
  EXPECT_ANY_THROW(str3.as_int()) << err;
  EXPECT_ANY_THROW(str3.as_list()) << err;
  EXPECT_ANY_THROW(str3.as_dict()) << err;
  EXPECT_EQ(str3.encode(), "0:") << err;
  EXPECT_EQ(str3, decode("0:")) << err;
}

TEST(Bencode, ConstructList) {
  err = "Bad (List) constructor";
  Entry::List hi{1, "two", Entry::List{}, Entry::Dict{}};
  Entry list1(hi);
  EXPECT_TRUE(list1.is_list()) << err;
  EXPECT_EQ(list1.as_list().size(), 4) << err;
  EXPECT_EQ(list1.type(), Entry::LIST) << err;
  EXPECT_ANY_THROW(list1.as_int()) << err;
  EXPECT_ANY_THROW(list1.as_str()) << err;
  EXPECT_ANY_THROW(list1.as_dict()) << err;
  EXPECT_EQ(list1.encode(), "li1e3:twoledee") << err;
  EXPECT_EQ(list1, decode("li1e3:twoledee")) << err;

  err = "Bad (List) operator=";
  Entry::List empty_lol;
  list1 = empty_lol;
  EXPECT_TRUE(list1.is_list()) << err;
  EXPECT_EQ(list1.as_list().size(), 0) << err;
  EXPECT_EQ(list1.type(), Entry::LIST) << err;
  EXPECT_ANY_THROW(list1.as_int()) << err;
  EXPECT_ANY_THROW(list1.as_str()) << err;
  EXPECT_ANY_THROW(list1.as_dict()) << err;
  EXPECT_EQ(list1.encode(), "le") << err;
  EXPECT_EQ(list1, decode("le")) << err;

  empty_lol.push_back(1);
  err = "Bad (List &&) constructor";
  Entry list2(std::move(empty_lol));
  EXPECT_TRUE(list2.is_list()) << err;
  EXPECT_EQ(list2.as_list().size(), 1) << err;
  EXPECT_EQ(list2.type(), Entry::LIST) << err;
  EXPECT_ANY_THROW(list2.as_int()) << err;
  EXPECT_ANY_THROW(list2.as_str()) << err;
  EXPECT_ANY_THROW(list2.as_dict()) << err;
  EXPECT_EQ(list2.encode(), "li1ee") << err;
  EXPECT_EQ(list2, decode("li1ee")) << err;

  hi.pop_back();
  hi.pop_back();
  hi.pop_back();

  err = "Bad (List &&) operator=";
  list1 = std::move(hi);
  EXPECT_TRUE(list1.is_list()) << err;
  EXPECT_EQ(list1.as_list().size(), 1) << err;
  EXPECT_EQ(list1.type(), Entry::LIST) << err;
  EXPECT_ANY_THROW(list1.as_int()) << err;
  EXPECT_ANY_THROW(list1.as_str()) << err;
  EXPECT_ANY_THROW(list1.as_dict()) << err;
  EXPECT_EQ(list1.encode(), "li1ee") << err;
  EXPECT_EQ(list1, decode("li1ee")) << err;

  EXPECT_EQ(list1, list2) << "lists should be equal in the end";
}

TEST(Bencode, ConstructDict) {
  Entry dict1(Entry::Dict{{"hello", "world"},
                       {"can i hold list", Entry::List{"sure"}}});
  EXPECT_TRUE(dict1.is_dict()) << err;
  EXPECT_EQ(dict1.as_dict().size(), 2) << err;
  EXPECT_EQ(dict1.type(), Entry::DICT) << err;
  EXPECT_ANY_THROW(dict1.as_int()) << err;
  EXPECT_ANY_THROW(dict1.as_str()) << err;
  EXPECT_ANY_THROW(dict1.as_list()) << err;
  EXPECT_EQ(dict1.encode(), "d15:can i hold listl4:suree5:hello5:worlde") << err;
  EXPECT_EQ(dict1, decode("d15:can i hold listl4:suree5:hello5:worlde")) << err;

  Entry::Dict uhhh{{"1", 1},
                   {"2", "two"},
                   {"3", Entry::List{3, 3, 3}},
                   {"4", Entry::Dict{{"wow", "deep"}}}};
  dict1 = uhhh;
  EXPECT_TRUE(dict1.is_dict()) << err;
  EXPECT_EQ(dict1.as_dict().size(), 4) << err;
  EXPECT_EQ(dict1.type(), Entry::DICT) << err;
  EXPECT_ANY_THROW(dict1.as_int()) << err;
  EXPECT_ANY_THROW(dict1.as_str()) << err;
  EXPECT_ANY_THROW(dict1.as_list()) << err;
  EXPECT_EQ(dict1.encode(), "d1:1i1e1:23:two1:3li3ei3ei3ee1:4d3:wow4:deepee")
      << err;
  EXPECT_EQ(dict1, decode("d1:1i1e1:23:two1:3li3ei3ei3ee1:4d3:wow4:deepee")) << err;

  Entry dict2(Entry::Dict{});
  EXPECT_TRUE(dict2.is_dict()) << err;
  EXPECT_EQ(dict2.as_dict().size(), 0) << err;
  EXPECT_EQ(dict2.type(), Entry::DICT) << err;
  EXPECT_ANY_THROW(dict2.as_int()) << err;
  EXPECT_ANY_THROW(dict2.as_str()) << err;
  EXPECT_ANY_THROW(dict2.as_list()) << err;
  EXPECT_EQ(dict2.encode(), "de") << err;
  EXPECT_EQ(dict2, decode("de")) << err;

  dict2 = Entry::Dict{{"ligma", "balls"}};
  EXPECT_TRUE(dict2.is_dict()) << err;
  EXPECT_EQ(dict2.as_dict().size(), 1) << err;
  EXPECT_EQ(dict2.type(), Entry::DICT) << err;
  EXPECT_ANY_THROW(dict2.as_int()) << err;
  EXPECT_ANY_THROW(dict2.as_str()) << err;
  EXPECT_ANY_THROW(dict2.as_list()) << err;
  EXPECT_EQ(dict2.encode(), "d5:ligma5:ballse") << err;
  EXPECT_EQ(dict2, decode("d5:ligma5:ballse")) << err;
}

TEST(Bencode, Encode) {
  EXPECT_EQ(Entry(static_cast<Entry::Int>(0)).encode(), "i0e");
  EXPECT_EQ(Entry(-1).encode(), "i-1e");
  EXPECT_EQ(Entry(1).encode(), "i1e");
  EXPECT_EQ(Entry(123456789).encode(), "i123456789e");
  EXPECT_EQ(Entry(123456789).encode(), "i123456789e");

  EXPECT_EQ(Entry("").encode(), "0:");
  EXPECT_EQ(Entry("abcd").encode(), "4:abcd");
  EXPECT_EQ(Entry(std::string(":\t\0\n\rqqq", 8)).encode(), std::string("8::\t\0\n\rqqq", 10));

  EXPECT_EQ(Entry(Entry::List{}).encode(), "le");
  EXPECT_EQ(Entry(Entry::List{2143, "fafd", Entry::List{1, 2}, Entry::Dict{{"one", 2}}}).encode(), "li2143e4:fafdli1ei2eed3:onei2eee");

  EXPECT_EQ(Entry(Entry::Dict{}).encode(), "de");
  EXPECT_EQ(Entry(Entry::Dict{{"different", 1}, {"ORDER", 2}}).encode(), "d5:ORDERi2e9:differenti1ee");
}

TEST(Bencode, DecodeErrors) {
  EXPECT_ANY_THROW(decode("")) << "Can't decode empty string";

  EXPECT_ANY_THROW(decode("i1ei2e")) << "Non singular root item";
  EXPECT_ANY_THROW(decode("i1e0:"))  << "Non singular root item";
  EXPECT_ANY_THROW(decode("i1ele"))  << "Non singular root item";
  EXPECT_ANY_THROW(decode("i1ede"))  << "Non singular root item";

  EXPECT_ANY_THROW(decode("0:i1e")) << "Non singular root item";
  EXPECT_ANY_THROW(decode("0:0:"))  << "Non singular root item";
  EXPECT_ANY_THROW(decode("0:le"))  << "Non singular root item";
  EXPECT_ANY_THROW(decode("0:de"))  << "Non singular root item";

  EXPECT_ANY_THROW(decode("lei1e")) << "Non singular root item";
  EXPECT_ANY_THROW(decode("le0:"))  << "Non singular root item";
  EXPECT_ANY_THROW(decode("lele"))  << "Non singular root item";
  EXPECT_ANY_THROW(decode("lede"))  << "Non singular root item";

  EXPECT_ANY_THROW(decode("dei1e")) << "Non singular root item";
  EXPECT_ANY_THROW(decode("de0:"))  << "Non singular root item";
  EXPECT_ANY_THROW(decode("dele"))  << "Non singular root item";
  EXPECT_ANY_THROW(decode("dede"))  << "Non singular root item";

  char s[] = "ie";
  for (int c = 0; c < 256; c++) {
    if (std::isdigit(c) || c == 'l' || c == 'i' || c == 'd') {
      continue;
    }
    s[0] = c;
    EXPECT_ANY_THROW(decode(std::string(s, 2))) << "Invalid type";
  }

  EXPECT_ANY_THROW(decode("i0")) << "Missing e";
  EXPECT_ANY_THROW(decode("li1e")) << "Missing e";
  EXPECT_ANY_THROW(decode("d1:ai1e")) << "Missing e";

  EXPECT_ANY_THROW(decode("ie")) << "Empty int";
  EXPECT_ANY_THROW(decode("i-0e")) << "Neg zero not allowed";
  EXPECT_ANY_THROW(decode("i01231e")) << "Leading zero not allowed";
  EXPECT_ANY_THROW(decode("i-01231e")) << "Leading zero not allowed";
  EXPECT_ANY_THROW(decode("i123d41e")) << "Non-digit char in int";
  EXPECT_ANY_THROW(decode("i123-41e")) << "Non-digit char in int";

  EXPECT_ANY_THROW(decode("-1:")) << "Neg length";
  EXPECT_ANY_THROW(decode("5abcde")) << "No colon";
  EXPECT_ANY_THROW(decode("5:abcd")) << "Unexpected EOF";
  EXPECT_ANY_THROW(decode("05:abcde")) << "Leading zero in length";
  EXPECT_ANY_THROW(decode("4:abcdv")) << "Length too small";

  EXPECT_ANY_THROW(decode("di1ei1ee")) << "Key isn't string";
  EXPECT_ANY_THROW(decode("dlei1ee")) << "Key isn't string";
  EXPECT_ANY_THROW(decode("ddei1ee")) << "Key isn't string";
  EXPECT_ANY_THROW(decode("d1:ai1e1:ai2ee")) << "Dublicate keys";
  EXPECT_ANY_THROW(decode("d1:ai1e4:abcdle1:ai2ee")) << "Dublicate keys";
  EXPECT_ANY_THROW(decode("d9:differenti1e5:ORDERi2ee")) << "Keys should be sorted";
  EXPECT_ANY_THROW(decode("d5:ORDERi2e9:differente")) << "Key with no value";
}
