#include "gtest/gtest.h"

#include "core/torrent.h"

static std::filesystem::path tests = TEST_DATA_DIR;

static const char debian_fname[] = "debian-13.0.0-amd64-netinst.iso.torrent";
static const char animal_farm_fname[] = "AnimalFarmByGeorgeOrwell_archive.torrent";

using namespace cactus;

TEST(Torrent, NameCollisionThrows) {
  EXPECT_ANY_THROW(Torrent::create(tests / "AnimalFarmByGeorgeOrwell/", tests / debian_fname));
  EXPECT_ANY_THROW(Torrent::create(tests, tests / animal_farm_fname));
}

TEST(Torrent, SingleFileVerification) {
  auto alice_torrent = Torrent::create(tests, tests / "alice.torrent", false);
  EXPECT_TRUE(alice_torrent.reverify());
}
