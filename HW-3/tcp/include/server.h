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

    constexpr uint16_t DEF_MAX_CON = 1024;

    class Server {
    public:
        Server();
        Server(const std::string& address, uint16_t port);

        Server(const Server&) = delete;
        Server& operator=(const Server&) = delete;

        Server(Server&& tmp) noexcept;
        Server& operator=(Server&& tmp) noexcept;

        void listen(std::string& address, uint16_t port);
        void close();

        Connection accept();

    private:
        bool is_valid() const;

        Descriptor fd_;
    };


}

#endif //SERVER_H
