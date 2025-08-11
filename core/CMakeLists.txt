add_library(
    core
    STATIC
    src/bencode.cpp
    src/sha1.cpp
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
