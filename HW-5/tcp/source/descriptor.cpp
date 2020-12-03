//
// Created by Osip Chin on 25.10.2020.
//

#include "descriptor.h"
#include "tcp_exception.h"

#include <unistd.h>
#include <stdexcept>
#include <utility>

Descriptor::Descriptor() : fd_(-1)
{
}

Descriptor::Descriptor(int fd) {
    set_fd(fd);
}

Descriptor::~Descriptor() {
    close();
}

Descriptor::Descriptor(Descriptor&& tmp) noexcept : fd_(tmp.fd_) {
    tmp.fd_ = -1;
}

Descriptor& Descriptor::operator=(Descriptor&& tmp) noexcept {
    close();
    fd_ = std::exchange(tmp.fd_, -1);
    return *this;
}

void Descriptor::close() {
    if (fd_ >= 0) {
        ::close(fd_);
        fd_ = -1;
    }
}

void Descriptor::set_fd(int fd) {
    if (fd < 0) {
        throw tcp::DescriptorError("invalid_descriptor");
    }
    close();
    fd_ = fd;
}

int Descriptor::get_fd() const {
    return fd_;
}
