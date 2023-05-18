cmake_minimum_required(VERSION 3.0)
project(include/logs_parser)
set(CMAKE_CXX_STANDARD 20)

include(FetchContent)
FetchContent_Declare(
        argparse
        GIT_REPOSITORY https://github.com/p-ranav/argparse.git
)
FetchContent_MakeAvailable(argparse)

FetchContent_Declare(json
        URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz
        )
FetchContent_MakeAvailable(json)

FetchContent_Declare(fmt
        GIT_REPOSITORY https://github.com/fmtlib/fmt.git
        GIT_TAG master
        )
FetchContent_MakeAvailable(fmt)

add_library(
        logs_parser
        sources/logs_parser/logs_parser.cpp
)

target_include_directories(
        logs_parser PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(logs_parser PUBLIC fmt::fmt nlohmann_json::nlohmann_json)

add_executable(cli cli/main.cpp)
target_link_libraries(cli argparse logs_parser)