add_library(
    core
    STATIC
    src/bencode.cpp
    src/file_manager.cpp
    src/metainfo.cpp
    src/sha1.cpp
    src/tracker.cpp
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
