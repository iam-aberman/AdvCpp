//
// Created by Osip Chin on 07.11.2020.
//
#include "shared_memory_map.h"
#include "shmem_exception.h"

#include <unistd.h>
#include <sys/mman.h>

namespace shmem {

    MemoryMap::MemoryMap(size_t size) : size_(size) {
        if (size_ == 0) {
            throw mem_map_error("zero_mem_size");
        }

        mem_ = ::mmap(nullptr, size_, PROT_READ | PROT_WRITE,
                      MAP_SHARED | MAP_ANONYMOUS, -1, 0);
        if (mem_ == MAP_FAILED) {
            throw mem_map_error("mem_map_failed");
        }
    }

    MemoryMap::~MemoryMap() {
        ::munmap(mem_, size_);
    }

    void* MemoryMap::get() {
        return mem_;
    }

}
