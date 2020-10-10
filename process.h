//
// Created by Osip Chin
//
#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include <sys/types.h>

class Process {
public:
    explicit Process(const std::string& path);
    ~Process();

    size_t write(const void* data, size_t len);
    void writeExact(const void* data, size_t len);
    size_t read(void* data, size_t len);
    void readExact(void* data, size_t len);

    void closeStdin();
    void close();

private:
    void closePipes();

    pid_t fork_pid_;

    // Pipes
    int parent_to_child_;
    int child_to_parent_;
};

#endif //PROCESS_H
