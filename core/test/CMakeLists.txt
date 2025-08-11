enable_testing()

add_executable(
    core_test
    test_sha1.cpp
    test_bencode.cpp
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

include(GoogleTest)
gtest_discover_tests(core_test)
