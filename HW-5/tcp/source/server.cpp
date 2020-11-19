//
// Created by Osip Chin on 29.10.2020.
//
#include "server.h"
#include "tcp_exception.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdexcept>

namespace tcp {

    Server::Server() : fd_(Descriptor())            // Invalid fd_ = -1;
    {
    }

    Server::Server(const std::string& address, uint16_t port) {
        fd_ = Descriptor( ::socket(PF_INET, SOCK_STREAM, 0));
        if (fd_.get_fd() < 0) {
            throw DescriptorError("bad_socket");
        }

        int opt = 1;
        if (::setsockopt(fd_.get_fd(), SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
            throw DescriptorError("bad_socket_opt");
        }

        sockaddr_in addr{};
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        if (::inet_aton(address.data(), &addr.sin_addr) < 0) {
            throw ConnectionError("bad_address");
        }

        if (::bind(fd_.get_fd(), reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0) {
            throw ConnectionError("bind_fail");
        }
        if (::listen(fd_.get_fd(), DEF_MAX_CON) < 0) {
            throw ConnectionError("connection_refused");
        }
    }

    Server::Server(Server&& tmp) noexcept : fd_(std::move(tmp.fd_))
    {
    }

    Server& Server::operator=(Server&& tmp) noexcept {
        close();
        fd_ = std::move(tmp.fd_);
        return *this;
    }

    void Server::listen(const std::string& address, uint16_t port) {
        if (is_valid()) {
            return;
        }

        Server tmp(address, port);
        std::swap(*this, tmp);
    }

    void Server::close() {
        fd_.close();
    }

    Connection Server::accept() {
        sockaddr_in client_addr{};
        socklen_t size = sizeof(client_addr);

        int client_fd = ::accept(fd_.get_fd(),
                                 reinterpret_cast<sockaddr*>(&client_addr), &size);
        if (client_fd < 0) {
            throw ConnectionError("accept_fail");
        } else {
            return Connection(Descriptor(client_fd));
        }
    }

    Descriptor& Server::get_fd() {
        return fd_;
    }

    bool Server::is_valid() const {
        return fd_.get_fd() >= 0;
    }

}
