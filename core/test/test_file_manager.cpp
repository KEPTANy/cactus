#include "gtest/gtest.h"

#include <fstream>

#include "core/file_manager.h"
#include "core/piece.h"

static std::filesystem::path tests = TEST_DATA_DIR;

std::string extract_from_file(const std::filesystem::path &path) {
  std::ifstream in(path, std::ios::binary);
  std::string res(std::filesystem::file_size(path), '\0');
  in.read(res.data(), res.size());
  return res;
}

TEST(FileManager, PieceRW) {
  cactus::Piece p1(3, 0);
  auto &v = p1.vec();
  v[0] = 'a';
  v[1] = 'b';
  v[2] = 'c';

  cactus::FileManager mgr;
  mgr.add_file("a.txt", 2);
  mgr.add_file("b.txt", 1);

  mgr.preallocate(tests / "piece_test");
  mgr.write_piece(tests / "piece_test", p1);

  EXPECT_EQ(extract_from_file(tests / "piece_test" / "a.txt"), "ab");
  EXPECT_EQ(extract_from_file(tests / "piece_test" / "b.txt"), "c");

  cactus::Piece p2(3, 0);
  mgr.read_piece(tests / "piece_test", p2);
  EXPECT_EQ(p1.vec(), p2.vec());

  cactus::Piece p3(2, 1);
  mgr.read_piece(tests / "piece_test", p3);
  EXPECT_EQ(p3.vec(), std::vector<std::uint8_t>({ 'b', 'c' }));

  std::filesystem::remove_all(tests / "piece_test");
}
