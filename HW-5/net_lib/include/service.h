#ifndef SERVICE_H
#define SERVICE_H

#include "buffered_connection.h"
#include "service_listener.h"
#include "server.h"
#include <unordered_map>
#include <memory>

namespace net {

    class Service {
    public:
        Service() = default;
        explicit Service(std::unique_ptr<IServiceListener>&& listener);

        Service(const Service&) = delete;
        Service& operator=(const Service&) = delete;

        Service(Service&&) = default;
        Service& operator=(Service&&) = default;

        void set_listener(std::unique_ptr<IServiceListener>&& listener);
        void open(const std::string& address, int port);
        void close();

        static void subscribe_to(BufferedConnection& con, uint32_t events);
        static void unsubscribe_from(BufferedConnection& con, uint32_t events);

        void run();
    private:
        std::unique_ptr<IServiceListener> listener_;

        tcp::Server server_;
        EPoll epoll_;
        std::unordered_map<int, BufferedConnection> active_connections_;

    };

}

#endif //SERVICE_H
