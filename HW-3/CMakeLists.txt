cmake_minimum_required(VERSION 3.16)

project(HW-3)
set(CMAKE_CXX_STANDARD 17)
add_compile_options(-Wall -Werror)

add_subdirectory(tcp)

add_executable(test test.cpp)
target_link_libraries(test tcp)
