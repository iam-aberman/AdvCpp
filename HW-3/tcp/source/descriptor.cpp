//
// Created by Osip Chin on 25.10.2020.
//

#include "descriptor.h"
#include "tcp_exception.h"

#include <unistd.h>
#include <stdexcept>

Descriptor::Descriptor() : fd_(-1)
{
}

Descriptor::Descriptor(int fd) :
        fd_(fd >= 0 ? fd : -1)
{
}

Descriptor::~Descriptor() {
    close();
}

Descriptor::Descriptor(Descriptor&& tmp) noexcept : fd_(tmp.fd_) {
    tmp.fd_ = -1;
}

Descriptor& Descriptor::operator=(Descriptor&& tmp) noexcept {
    std::swap(this->fd_, tmp.fd_);
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
        throw tcp::descriptor_error("invalid_descriptor");
    }
    close();
    fd_ = fd;
}

int Descriptor::get_fd() const {
    return fd_;
}
