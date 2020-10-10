//
// Created by Osip Chin
//
#include "process.h"

#include <stdexcept>
#include <unistd.h>
#include <csignal>

#define PIPE_SIZE 2

Process::Process(const std::string& path) {
    if (pipe(parent_to_child_) == -1) {
        exit(1);
    }
    if (pipe(child_to_parent_) == -1) {
        ::close(parent_to_child_[0]);
        ::close(parent_to_child_[1]);
        exit(2);
    }

    fork_pid_ = fork();
    if (fork_pid_ < 0) {
        this->closePipes();
        exit(3);
    }

    if (!fork_pid_) {                                   // This is child-process
        dup2(parent_to_child_[0], STDIN_FILENO);
        dup2(child_to_parent_[1], STDOUT_FILENO);
        this->closePipes();

        std::string exec_path(path);
        char* args[2] = {exec_path.data(), nullptr};
        execvp(args[0], args);
    } else {                                            // This is parent-process
        ::close(parent_to_child_[0]);
        parent_to_child_[0] = -1;
        ::close(child_to_parent_[1]);
        child_to_parent_[1] = -1;
    }
}

size_t Process::write(const void* data, size_t len) {
    ssize_t res = ::write(parent_to_child_[1], data, len);
    if (res >= 0) {
        return static_cast<size_t>(res);
    } else {
        throw std::runtime_error("BAD_WR");
    }
}

void Process::writeExact(const void* data, size_t len) {
    size_t written = 0;
    ssize_t step;
    while (written != len) {
        step = ::write(parent_to_child_[1], static_cast<const char*>(data) + written, 1);
        if (step >= 0) {
            written += static_cast<size_t>(step);
        } else {
            throw std::runtime_error("BAD_WR_EXACT");
        }
    }
}

size_t Process::read(void* data, size_t len) {
    int res = ::read(child_to_parent_[0], data, len);
    if (res >= 0) {
        return static_cast<size_t>(res);
    } else {
        throw std::runtime_error("BAD_RD");
    }
}

void Process::readExact(void* data, size_t len) {
    size_t read = 0;
    ssize_t step;
    while (read != len) {
        step = ::read(child_to_parent_[0], static_cast<char*>(data) + read, 1);
        if (step >= 0) {
            read += static_cast<size_t>(step);
        } else {
            throw std::runtime_error("BAD_RD_EXACT");
        }
    }
}

void Process::closeStdin() {
    if (fork_pid_) {                                    // Parent
        if (parent_to_child_[1] >= 0) {
            ::close(parent_to_child_[1]);
            parent_to_child_[1] = -1;
        }
    }
}

void Process::close() {
    if (fork_pid_) {
        int status;
        int child_state = waitpid(fork_pid_, &status, WNOHANG);

        if (child_state) {                              // Process is already done, nothing to close
            return;
        } else {                                        // Still running
            ::kill(fork_pid_, SIGINT);
        }
    }
}

void Process::closePipes() {
    for (int i = 0; i < PIPE_SIZE; ++i) {
        if (parent_to_child_[i] >= 0) {
            ::close(parent_to_child_[i]);
            parent_to_child_[i] = -1;
        }
        if (child_to_parent_[i] >= 0) {
            ::close(child_to_parent_[i]);
            child_to_parent_[i] = -1;
        }
    }
}

Process::~Process() {
    if (fork_pid_) {
        this->closePipes();
        this->close();
    }
}

#undef PIPE_SIZE
