#include "core/sha1.h"
#include "gtest/gtest.h"

#include <cstring>

static std::string operator*(std::string str, std::size_t n) {
  std::string res{};
  while (n--) {
    res += str;
  }

  return res;
}

TEST(SHA1, Copy) {
  std::array<uint8_t, 20> x{0xa9, 0x99, 0x3e, 0x36, 0x47, 0x06, 0x81,
                            0x6a, 0xba, 0x3e, 0x25, 0x71, 0x78, 0x50,
                            0xc2, 0x6c, 0x9c, 0xd0, 0xd8, 0x9d};

  auto hash = cactus::SHA1::copy(x.data());

  EXPECT_EQ(std::memcmp(hash.begin(), x.data(), cactus::SHA1::hash_size), 0)
      << "Failed to copy initialize SHA1 hash";
  EXPECT_EQ(hash.hex(), "a9993e364706816aba3e25717850c26c9cd0d89d");
}

TEST(SHA1, FromHex) {
  std::array<uint8_t, 20> x{0xa9, 0x99, 0x3e, 0x36, 0x47, 0x06, 0x81,
                            0x6a, 0xba, 0x3e, 0x25, 0x71, 0x78, 0x50,
                            0xc2, 0x6c, 0x9c, 0xd0, 0xd8, 0x9d};
  auto hash = cactus::SHA1::from_hex_string("a9993e364706816aba3e25717850c26c9cd0d89d");

  EXPECT_EQ(std::memcmp(hash.begin(), x.data(), cactus::SHA1::hash_size), 0)
      << "Failed to copy initialize SHA1 hash";
  EXPECT_EQ(hash.hex(), "a9993e364706816aba3e25717850c26c9cd0d89d");
}

TEST(SHA1, Comparisons) {
  std::array<uint8_t, 20> x{0xa9, 0x99, 0x3e, 0x36, 0x47, 0x06, 0x81,
                            0x6a, 0xba, 0x3e, 0x25, 0x71, 0x78, 0x50,
                            0xc2, 0x6c, 0x9c, 0xd0, 0xd8, 0x9d};

  auto hash1 = cactus::SHA1::copy(x.data());
  x[0] += 10;
  auto hash2 = cactus::SHA1::copy(x.data());
  x[0] -= 10;
  auto hash3 = cactus::SHA1::copy(x.data());

  EXPECT_TRUE(hash1 == hash3);
  EXPECT_TRUE(hash2 != hash3);
  EXPECT_TRUE(hash1 != hash2);

  EXPECT_EQ(hash1.hex(), "a9993e364706816aba3e25717850c26c9cd0d89d");
  EXPECT_EQ(hash2.hex(), "b3993e364706816aba3e25717850c26c9cd0d89d");
  EXPECT_EQ(hash3.hex(), "a9993e364706816aba3e25717850c26c9cd0d89d");
}

TEST(SHA1, HashComputation) {
  // https://www.di-mgt.com.au/sha_testvectors.html
  // clang-format off
  std::vector<std::tuple<std::string, std::string>> test_cases{
    {"abc",
       "a9993e364706816aba3e25717850c26c9cd0d89d"},

    {"",
       "da39a3ee5e6b4b0d3255bfef95601890afd80709"},

    {"abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq",
       "84983e441c3bd26ebaae4aa1f95129e5e54670f1"},

    {"abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu",
       "a49b2446a02c645bf419f995b67091253a04a259"},

    {std::string("a") * 1000000,
       "34aa973cd4c4daa4f61eeb2bdbad27316534016f"},

    {std::string("abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmno") * 16777216,
       "7789f0c9ef7bfc40d93311143dfbe69e2017f592"}
  };
  // clang-format on

  for (std::size_t i{0}; i < test_cases.size(); i++) {
    auto &[str, hex] = test_cases[i];
    auto hash = cactus::SHA1::compute(str.data(), str.size());
    EXPECT_EQ(hash, cactus::SHA1::from_hex_string(hex));
  }
}
