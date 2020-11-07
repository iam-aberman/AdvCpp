add_library(shmem source/smth.cpp
            source/shmem_semaphore.cpp
            source/shmem_exception.cpp
            source/shared_map.cpp source/shared_memory_map.cpp)
target_include_directories(shmem PUBLIC include)