#include <iostream>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include "shmem_semaphore.h"


int main() {

    void* shmem = ::mmap(nullptr, 1024, PROT_READ | PROT_WRITE,
                         MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    auto* sem = new (shmem) shmem::Semaphore;

    auto* buffer = new (static_cast<char*>(shmem) + sizeof(shmem::Semaphore)) int;


    pid_t child = ::fork();
    if (child > 0) { // Parent
        sem->wait();
        *buffer = 7777;
        ::sleep(10);

        std::cerr << *buffer << std::endl;
        sem->post();

        ::waitpid(child, nullptr, 0);
    } else {
        ::sleep(5);
        sem->wait();
        *buffer = 8888;
        sem->post();
        return 0;
    }

    munmap(shmem, 1024);
    return 0;
}
