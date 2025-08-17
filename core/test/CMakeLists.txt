add_executable(
    core_test
    test_sha1.cpp
    test_bencode.cpp
    test_metainfo.cpp
    test_torrent.cpp
)

target_include_directories(
    core_test
    PUBLIC
    ../include
)

target_link_libraries(
    core_test
    PRIVATE
    core
    GTest::gtest_main
)

set(TEST_DATA_DIR "${CMAKE_CURRENT_BINARY_DIR}/test_data")

add_custom_command(TARGET core_test POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_CURRENT_SOURCE_DIR}/test_data/"
    "${TEST_DATA_DIR}")
enable_testing()

target_compile_definitions(
    core_test
    PRIVATE
    TEST_DATA_DIR="${TEST_DATA_DIR}"
)

include(GoogleTest)
gtest_discover_tests(core_test)
