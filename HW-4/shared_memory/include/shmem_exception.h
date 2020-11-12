//
// Created by Osip Chin on 06.11.2020.
//
#ifndef SHMEM_EXCEPTION_H
#define SHMEM_EXCEPTION_H

#include <stdexcept>

namespace shmem {

    class ShError : public std::runtime_error {
        using std::runtime_error::runtime_error;
    };

    class SemaphoreError : public ShError {
    public:
        using ShError::ShError;
    };

    class MemMapError : public ShError {
    public:
        using ShError::ShError;
    };

    class OutOfRange : public ShError {
        using ShError::ShError;
    };

} // namespace shmem

#endif //SHMEM_EXCEPTION_H
