//
// Created by Osip Chin on 07.11.2020.
//
#ifndef SHARED_MEMORY_MAP_H
#define SHARED_MEMORY_MAP_H

#include <cstddef>

namespace shmem {

    class MemoryMap {
    public:
        explicit MemoryMap(size_t size);
        ~MemoryMap();

        MemoryMap(const MemoryMap&) = delete;
        MemoryMap& operator=(const MemoryMap&) = delete;

        MemoryMap(MemoryMap&&) = delete;
        MemoryMap& operator=(MemoryMap&&) = delete;

        char* get();

    private:
        void* mem_;
        size_t size_;
    };

} // namespace shmem

#endif //SHARED_MEMORY_MAP_H
