# BitTorrent client implemented in C++

# How to build

First of all, you need to install the following dependencies:
```
OpenSSL
CMake
some c++20 compatible compiler and make (duh, who would've guessed)
```

Then you should be good to go. While building, ensure your internet connection
is stable, CMake will fetch GoogleTest. Run the following:
```bash
mkdir build/
cmake ..
make
```

Optionally, you might want to run the test suite, just run `ctest` in `build/`
directory.

# Resources

- [theory.org's BitTorrentSpecification](https://wiki.theory.org/BitTorrentSpecification)
- [The BitTorrent Protocol Specification](https://www.bittorrent.org/beps/bep_0003.html)
