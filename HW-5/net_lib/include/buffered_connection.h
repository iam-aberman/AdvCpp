#ifndef BUFFERED_CONNECTION_H
#define BUFFERED_CONNECTION_H

#include "net_epoll.h"
#include "connection.h"
#include <string>

namespace net {
    class Service;

    class BufferedConnection {
    public:
        BufferedConnection() = delete;

        BufferedConnection(tcp::Connection&& tmp, EPoll* epoll);
        BufferedConnection(BufferedConnection&&) = default;
        BufferedConnection& operator=(BufferedConnection&&) = default;

        BufferedConnection(const BufferedConnection&) = delete;
        BufferedConnection& operator=(const BufferedConnection&) = delete;

        void read(std::string& dest);
        void write(const std::string& msg);

        std::string& get_read_buf();
        std::string& get_write_buf();
        Descriptor& get_fd();

        void subscribe(uint32_t events);
        void unsubscribe(uint32_t events);
        void close();
    private:
        friend class Service;
        size_t read_from_buf();
        size_t write_to_buf();

        // Buffers
        std::string read_buf_,
                    write_buf_;

        tcp::Connection connection_;

        EPoll* epoll_;
        uint32_t epoll_flags_;
    };

} // namespace net

#endif //BUFFERED_CONNECTION_H
