//
// Created by Osip Chin on 06.11.2020.
//
#include "shared_semaphore.h"
#include "shmem_exception.h"

#include <iostream>

namespace shmem {

    Semaphore::Semaphore() {
        if (::sem_init(&sem_, 1, 1) < 0) {
            throw semaphore_error("sem_init_fail");
        }
    }

    Semaphore::~Semaphore() {
        if (::sem_destroy(&sem_) < 0) {
            std::cerr << "sem_destroy_fail\n";
        }
    }

    void Semaphore::wait() {
        ::sem_wait(&sem_);
    }

    void Semaphore::post() {
        ::sem_post(&sem_);
    }

} // namespace shmem
