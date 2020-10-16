//
// Created by Osip Chin
//
#include "process.h"

#include <vector>
#include <stdexcept>
#include <csignal>
#include <unistd.h>
#include <sys/wait.h>

constexpr size_t PIPE_SIZE = 2u;

Process::Process(const std::string& path, std::vector<std::string> arguments) {  // Passing by value to avoid extra-copies
    int parent_to_child[2], child_to_parent[2];                                  // May be modified

    if (::pipe(parent_to_child) == -1) {
        throw std::runtime_error("BAD_PIPE");
    }
    if (::pipe(child_to_parent) == -1) {
        ::close(parent_to_child[0]);
        ::close(parent_to_child[1]);
        throw std::runtime_error("BAD_PIPE");
    }

    fork_pid_ = fork();
    if (fork_pid_ < 0) {
        for (int i = 0; i < PIPE_SIZE; ++i) {
            ::close(parent_to_child[i]);
            ::close(child_to_parent[i]);
        }

        throw std::runtime_error("BAD_FORK");
    }

    if (!fork_pid_) {                                   // This is child-process
        ::dup2(parent_to_child[0], STDIN_FILENO);
        ::dup2(child_to_parent[1], STDOUT_FILENO);
        for (int i = 0; i < PIPE_SIZE; ++i) {
            ::close(parent_to_child[i]);
            ::close(child_to_parent[i]);
        }
        parent_to_child_ = -1;
        child_to_parent_ = -1;

        std::vector<char*> args;
        std::string path_to_exec(path);
        args.reserve(arguments.size() + 1);

        for (std::string& arg : arguments) {
            if (arg.empty()) {
                ::exit(1);
            }
            args.push_back(arg.data());
        }
        args[arguments.size()] = nullptr;
        execvp(path_to_exec.data(), args.data());

        ::exit(2);
    } else {                                            // This is parent-process
        ::close(parent_to_child[0]);
        ::close(child_to_parent[1]);

        parent_to_child_ = parent_to_child[1];
        child_to_parent_ = child_to_parent[0];
    }
}

size_t Process::write(const void* data, size_t len) {
    ssize_t res = ::write(parent_to_child_, data, len);
    if (res >= 0) {
        return static_cast<size_t>(res);
    } else {
        throw std::runtime_error("BAD_WR");
    }
}

void Process::writeExact(const void* data, size_t len) {
    size_t written = 0;
    while (written != len) {
        written += write(static_cast<const char*>(data) + written, len - written);
    }
}

size_t Process::read(void* data, size_t len) {
    int res = ::read(child_to_parent_, data, len);
    if (res >= 0) {
        return static_cast<size_t>(res);
    } else {
        throw std::runtime_error("BAD_RD");
    }
}

void Process::readExact(void* data, size_t len) {
    size_t covered = 0;
    size_t step;
    while (covered != len) {
        step = read(static_cast<char*>(data) + covered, len - covered);
        if (step == 0) {
            throw std::runtime_error("BAD_RD");
        }
        covered += step;
    }
}

void Process::closeStdin() {
    if (fork_pid_) {                                    // Parent
        if (parent_to_child_ >= 0) {
            ::close(parent_to_child_);
            parent_to_child_ = -1;
        }
    }
}

void Process::close() {
    if (fork_pid_) {
        int status;
        ::kill(fork_pid_, SIGINT);
        ::waitpid(fork_pid_, &status, 0);

        if (status != 0) {
            throw std::runtime_error("EXEC_ERROR");
        }
    }
}

void Process::closePipes() {
    if (parent_to_child_ >= 0) {
        ::close(parent_to_child_);
        parent_to_child_ = -1;
    }
    if (child_to_parent_ >= 0) {
        ::close(child_to_parent_);
        child_to_parent_ = -1;
    }
}

Process::~Process() {
    if (fork_pid_) {
        closePipes();
        close();
    }
}
