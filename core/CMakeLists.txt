add_library(
    core
    STATIC
    src/bencode.cpp
    src/crypto.cpp
    src/file_manager.cpp
    src/metainfo.cpp
    src/piece_manager.cpp
    src/torrent.cpp
    src/tracker_manager.cpp
)

target_include_directories(
    core
    PUBLIC
    include
)

target_link_libraries(
    core
    PRIVATE
    OpenSSL::SSL
)

add_subdirectory(test)
