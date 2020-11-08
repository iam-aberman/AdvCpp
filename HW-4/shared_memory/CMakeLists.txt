add_library(shmem
            source/shared_semaphore.cpp
            source/shared_map.cpp
            source/shared_memory_map.cpp)
target_include_directories(shmem PUBLIC include)