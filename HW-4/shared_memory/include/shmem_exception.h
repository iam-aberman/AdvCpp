//
// Created by Osip Chin on 06.11.2020.
//
#ifndef SHMEM_EXCEPTION_H
#define SHMEM_EXCEPTION_H

#include <stdexcept>

namespace shmem {

    class semaphore_error : public std::runtime_error {
    public:
        using std::runtime_error::runtime_error;
    };

    class mem_map_error : public std::runtime_error {
    public:
        using std::runtime_error::runtime_error;
    };

    class out_of_range : public std::out_of_range {
        using std::out_of_range::out_of_range;
    };

} // namespace shmem

#endif //SHMEM_EXCEPTION_H
