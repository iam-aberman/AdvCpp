cmake_minimum_required(VERSION 3.10)
project(HW-4)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS -pthread)
set(CMAKE_EXE_LINKER_FLAGS -pthread)

add_subdirectory(shared_memory)
add_executable(main main.cpp)
target_link_libraries(main shmem)