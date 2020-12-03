//
// Created by Osip Chin on 25.10.2020.
//
#include "connection.h"
#include "tcp_exception.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdexcept>

#include <iostream>

namespace tcp {

    Connection::Connection(const std::string& address, uint16_t port) {
        connect(address, port);
    }

    void Connection::connect(const std::string& address, uint16_t port) {
        if (is_valid()) {
            throw ConnectionError("occupied_connection");
        }

        fd_ = Descriptor(::socket(PF_INET, SOCK_STREAM, 0));
        if (fd_.get_fd() < 0) {
            throw DescriptorError("bad_socket");
        }

        ::sockaddr_in addr{};
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        if (::inet_aton(address.data(), &addr.sin_addr) == 0) {
            throw ConnectionError("bad_address");
        }

        if (::connect(fd_.get_fd(), reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0) {
            throw ConnectionError("connection_fail");
        }
    }

    void Connection::close() {
        fd_.close();
    }

    size_t Connection::write(const void* data, size_t len) {
        if (!is_valid()) {
            throw DescriptorError("invalid_descriptor");
        }

        ssize_t written = ::write(fd_.get_fd(), data, len);
        if (written >= 0) {
            return static_cast<size_t>(written);
        } else {
            throw DescriptorError("write_fail");
        }
    }

    void Connection::write_exact(const void* data, size_t len) {
        size_t offset = 0;
        while (offset != len) {
            offset += write(static_cast<const char*>(data) + offset, len - offset);
        }
    }

    size_t Connection::read(void* data, size_t len) {
        if (!is_valid()) {
            throw DescriptorError("invalid_descriptor");
        }

        ssize_t read = ::read(fd_.get_fd(), data, len);
        if (read >= 0) {
            return static_cast<size_t>(read);
        } else {
            throw DescriptorError("read_fail");
        }
    }

    void Connection::read_exact(void* data, size_t len) {
        size_t offset = 0;
        while (offset != len) {
            size_t step = read(static_cast<char*>(data) + offset, len - offset);
            if (step == 0) {
                throw DescriptorError("read_exact_fail");
            }
            offset += step;
        }
    }

    void Connection::set_timeout(size_t ms) {
       if (!is_valid()) {
           return;
       }

        timeval timeout{.tv_sec = static_cast<int>(ms / 1000),
                        .tv_usec = static_cast<int>(ms % 1000)};
        for (int flag : {SO_SNDTIMEO, SO_RCVTIMEO}) {
            if (::setsockopt(fd_.get_fd(), SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0) {
                throw DescriptorError("bad_sock_opt");
            }
        }
    }

    Descriptor& Connection::get_fd() {
        return fd_;
    }

    Connection::Connection(Descriptor&& tmp_fd) : fd_(std::move(tmp_fd))
    {
    }

    bool Connection::is_valid() const {
        return fd_.get_fd() >= 0;
    }

}

