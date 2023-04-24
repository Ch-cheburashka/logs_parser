cmake_minimum_required(VERSION 3.0)
project(include/logs_parser)
set(CMAKE_CXX_STANDARD 20)

include(FetchContent)
FetchContent_Declare(
        argparse
        GIT_REPOSITORY https://github.com/p-ranav/argparse.git
)
FetchContent_MakeAvailable(argparse)

find_package(nlohmann_json 3.7.0 REQUIRED)

add_library(
        logs_parser
        sources/logs_parser/logs_parser.cpp
)

target_include_directories(
        logs_parser PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

add_executable(cli cli/main.cpp)
target_link_libraries(cli argparse)
target_link_libraries(cli nlohmann_json::nlohmann_json)
target_link_libraries(cli logs_parser)