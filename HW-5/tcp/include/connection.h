//
// Created by Osip Chin on 25.10.2020.
//

#ifndef CONNECTION_H
#define CONNECTION_H

#include "descriptor.h"

#include <string>
#include <cstdint>

namespace tcp {
    class Server;

    class Connection {
    public:
        Connection() = default;
        Connection(const std::string& address, uint16_t port);

        void connect(const std::string& address, uint16_t port);
        void close();

        size_t write(const void* data, size_t len);
        void write_exact(const void* data, size_t len);
        size_t read(void* data, size_t len);
        void read_exact(void* data, size_t len);

        void set_timeout(size_t ms);
        Descriptor& get_fd();

        friend Server;

    private:
        explicit Connection(Descriptor&& tmp_fd);
        bool is_valid() const;

        Descriptor fd_;
    };

}

#endif //CONNECTION_H
