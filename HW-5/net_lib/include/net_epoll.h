#ifndef NET_EPOLL_H
#define NET_EPOLL_H

#include "descriptor.h"

#include <sys/epoll.h>
#include <cstdint>
#include <vector>

namespace net {

    constexpr size_t DEF_EPOLL_SIZE = 1024u;

    class EPoll {
    public:
        EPoll();
        ~EPoll() = default;

        EPoll(const EPoll&) = delete;
        EPoll& operator= (const EPoll&) = delete;

        EPoll(EPoll&&) = default;
        EPoll& operator=(EPoll&&) = default;

        void add(Descriptor& fd, uint32_t events);
        void mod(Descriptor& fd, uint32_t events);
        void del(Descriptor& fd);

        std::vector<::epoll_event> wait();
    private:
        Descriptor epoll_fd_;
        size_t max_size_;
    };

}

#endif //NET_EPOLL_H
