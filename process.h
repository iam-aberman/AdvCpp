//
// Created by Osip Chin
//
#ifndef PROCESS_H
#define PROCESS_H

#include <string>

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

    int fork_pid_;
    // Pipelines
    int parent_to_child_[2];
    int child_to_parent_[2];

    bool is_stdin_closed_ = false;
};

#endif //PROCESS_H
