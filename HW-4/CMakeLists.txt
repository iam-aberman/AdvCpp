cmake_minimum_required(VERSION 3.10)
project(HW-4)

set(CMAKE_CXX_STANDARD 17)
find_package(Threads)

add_subdirectory(shared_memory)
add_executable(main main.cpp)

target_link_libraries(main ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(main shmem)
