add_library(
    core
    STATIC
    src/bencode.cpp
)

target_include_directories(
    core
    PUBLIC
    include
)
