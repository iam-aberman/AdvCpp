cmake_minimum_required(VERSION 3.16)

project(test_logger)

set(CMAKE_CXX_STANDARD 17)


add_executable(test_logger main.cpp)
add_subdirectory(Log)
target_link_libraries(test_logger log_lib)