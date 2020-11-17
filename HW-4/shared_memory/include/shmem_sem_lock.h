//
// Created by Osip Chin on 06.11.2020.
//
#ifndef SHMEM_SEM_LOCK_H
#define SHMEM_SEM_LOCK_H

#include "shared_semaphore.h"

namespace shmem {

    class SemLock {
    public:
        explicit SemLock(Semaphore& sem) : sem_(sem){
            sem_.wait();
        }

        ~SemLock() {
            sem_.post();
        }

    private:
        Semaphore& sem_;
    };

} // namespace shmem

#endif //SHMEM_SEM_LOCK_H
