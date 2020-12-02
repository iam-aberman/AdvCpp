cmake_minimum_required(VERSION 3.10)
project(HW_5)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(net_lib)
add_subdirectory(tcp)

add_executable(HW_5 main.cpp example_listener.h)
target_link_libraries(HW_5 net)
target_link_libraries(HW_5 tcp)
