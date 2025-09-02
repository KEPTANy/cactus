cmake_minimum_required(VERSION 3.16)

project(cactus LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(CTest)
include(FetchContent)
fetchcontent_declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.17.0
)
fetchcontent_makeavailable(googletest)

find_package(OpenSSL REQUIRED)

FetchContent_Declare(
    asio
    GIT_REPOSITORY https://github.com/chriskohlhoff/asio.git
    GIT_TAG        asio-1-36-0
)
FetchContent_MakeAvailable(asio)
add_compile_definitions(ASIO_STANDALONE)

include(FetchContent)
FetchContent_Declare(
    cpr
    GIT_REPOSITORY
    https://github.com/libcpr/cpr.git
    GIT_TAG 1.12.0
)
FetchContent_MakeAvailable(cpr)

add_subdirectory(core)
