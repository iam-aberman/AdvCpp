#ifndef NET_EPOLL_H
#define NET_EPOLL_H

#include "descriptor.h"

#include <sys/epoll.h>
#include <cstdint>
#include <vector>

namespace net {

    class EPoll {
    public:
        explicit EPoll();
        ~EPoll() = default;

        EPoll(EPoll&& tmp) noexcept;
        EPoll& operator=(EPoll&& tmp) noexcept;

        void set_max_size(size_t new_size);

        void add(const Descriptor& fd, uint32_t events = 0u);
        void mod(const Descriptor& fd, uint32_t events);
        void del(const Descriptor& fd);

        std::vector<::epoll_event> wait();
    private:
        Descriptor epoll_fd_;
        size_t max_size_;
    };

} // namespace net

#endif //NET_EPOLL_H
