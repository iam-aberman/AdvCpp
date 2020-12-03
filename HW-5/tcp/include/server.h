//
// Created by Osip Chin on 29.10.2020.
//

#ifndef SERVER_H
#define SERVER_H

#include "descriptor.h"
#include "connection.h"

#include <string>
#include <cstdint>

namespace tcp {

    class Server {
    public:
        Server() = default;
        Server(const std::string& address, uint16_t port);

        void listen(const std::string& address, uint16_t port);
        void close();

        Connection accept();

        Descriptor& get_fd();

    private:
        bool is_valid() const;

        Descriptor fd_;
    };

}

#endif //SERVER_H
