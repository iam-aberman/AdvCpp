//
// Created by Osip Chin on 06.11.2020.
//
#ifndef SHMEM_SEMAPHORE_H
#define SHMEM_SEMAPHORE_H

#include <semaphore.h>

namespace shmem {

    class Semaphore {
    public:
        Semaphore();
        ~Semaphore();

        Semaphore(const Semaphore&) = delete;
        Semaphore &operator=(const Semaphore&) = delete;

        Semaphore(Semaphore&&) = delete;
        Semaphore &operator=(Semaphore&&) = delete;

        void wait();
        void post();

    private:
        sem_t sem_;
    };

} // namespace shmem

#endif //SHMEM_SEMAPHORE_H
