#include "gtest/gtest.h"

#include "core/metainfo.h"

#include <filesystem>

std::filesystem::path tests = TEST_DATA_DIR;

const char debian_fname[] = "debian-13.0.0-amd64-netinst.iso.torrent";
const char animal_farm_fname[] = "AnimalFarmByGeorgeOrwell_archive.torrent";

TEST(Metainfo, Parsing) {
  {
    auto debian = cactus::Metainfo::from_file(tests / debian_fname);
    EXPECT_EQ(debian.info_hash(),
              cactus::SHA1::from_hex_string(
                  "155a51b44b337d3b147c8d93d9764df48705ff89"));
    EXPECT_EQ(debian.tracker().url(),
              "http://bttracker.debian.org:6969/announce");
    EXPECT_EQ(debian.name(), "debian-13.0.0-amd64-netinst.iso");
    EXPECT_EQ(debian.creator(), "mktorrent 1.1");
    EXPECT_EQ(debian.comment(), "Debian CD from cdimage.debian.org");
    EXPECT_EQ(debian.creation_time(), 1754746007);
    EXPECT_EQ(debian.piece_size(), 262144);
    EXPECT_EQ(debian.storage().total_size(), 790626304);
    const auto &files = debian.storage().files();
    std::vector<std::tuple<const char *, std::size_t, std::size_t>> expected{
        {"debian-13.0.0-amd64-netinst.iso", 790626304, 0}};
    EXPECT_EQ(files.size(), expected.size());
    for (std::size_t i = 0; i < files.size(); i++) {
      auto [exp_fname, exp_size, exp_offset] = expected[i];
      EXPECT_EQ(files[i].fname, exp_fname);
      EXPECT_EQ(files[i].size, exp_size);
      EXPECT_EQ(files[i].offset, exp_offset);
    }
  }

  {
    auto animal_farm = cactus::Metainfo::from_file(tests / animal_farm_fname);
    EXPECT_EQ(animal_farm.info_hash(),
              cactus::SHA1::from_hex_string(
                  "246551a33080b1c4827d09466f22380d78a667c8"));
    EXPECT_EQ(animal_farm.tracker().url(),
              "http://bt1.archive.org:6969/announce");
    EXPECT_EQ(animal_farm.name(), "AnimalFarmByGeorgeOrwell");
    EXPECT_EQ(animal_farm.creator(), "ia_make_torrent");
    EXPECT_EQ(animal_farm.creation_time(), 1736166308);
    EXPECT_EQ(animal_farm.piece_size(), 524288);
    EXPECT_EQ(animal_farm.storage().total_size(), 62820568);
    const auto &files = animal_farm.storage().files();
    std::vector<std::tuple<const char *, std::size_t, std::size_t>> expected{
        {"AnimalFarmByGeorgeOrwell/Animal Farm by George Orwell.epub", 127971, 0},
        {"AnimalFarmByGeorgeOrwell/Animal Farm by George Orwell.mobi", 164079, 127971},
        {"AnimalFarmByGeorgeOrwell/Animal Farm by George Orwell.pdf", 474025, 292050},
        {"AnimalFarmByGeorgeOrwell/Animal Farm by George Orwell_abbyy.gz", 2266942, 766075},
        {"AnimalFarmByGeorgeOrwell/Animal Farm by George Orwell_daisy.zip", 111373, 3033017},
        {"AnimalFarmByGeorgeOrwell/Animal Farm by George Orwell_djvu.txt", 175665, 3144390},
        {"AnimalFarmByGeorgeOrwell/Animal Farm by George Orwell_djvu.xml", 1691837, 3320055},
        {"AnimalFarmByGeorgeOrwell/Animal Farm by George Orwell_jp2.zip", 57746785, 5011892},
        {"AnimalFarmByGeorgeOrwell/Animal Farm by George Orwell_scandata.xml", 32527, 62758677},
        {"AnimalFarmByGeorgeOrwell/AnimalFarmByGeorgeOrwell_meta.sqlite", 23552, 62791204},
        {"AnimalFarmByGeorgeOrwell/AnimalFarmByGeorgeOrwell_meta.xml", 2010, 62814756},
        {"AnimalFarmByGeorgeOrwell/__ia_thumb.jpg", 3802, 62816766},
    };
    EXPECT_EQ(files.size(), expected.size());
    for (std::size_t i = 0; i < files.size(); i++) {
      auto [exp_fname, exp_size, exp_offset] = expected[i];
      EXPECT_EQ(files[i].fname, exp_fname);
      EXPECT_EQ(files[i].size, exp_size);
      EXPECT_EQ(files[i].offset, exp_offset);
    }
  }
}
