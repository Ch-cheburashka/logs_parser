cmake_minimum_required(VERSION 3.0)
project(logs_parser)
set(CMAKE_CXX_STANDARD 20)

include(FetchContent)
FetchContent_Declare(
        argparse
        GIT_REPOSITORY https://github.com/p-ranav/argparse.git
)
FetchContent_MakeAvailable(argparse)

add_executable(cli cli/main.cpp)
target_link_libraries(cli argparse)