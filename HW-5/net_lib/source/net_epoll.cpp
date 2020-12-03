#include "net_exception.h"
#include "net_epoll.h"

namespace net {

    constexpr size_t DEF_EPOLL_SIZE = 1024u;

    EPoll::EPoll() : max_size_(DEF_EPOLL_SIZE),
                     epoll_fd_(Descriptor(::epoll_create(1)))
    {
    }

    EPoll::EPoll(EPoll&& tmp) noexcept {
        epoll_fd_ = std::move(tmp.epoll_fd_);
        max_size_ = DEF_EPOLL_SIZE;
    }
    EPoll& EPoll::operator=(EPoll&& tmp) noexcept {
        epoll_fd_ = std::move(tmp.epoll_fd_);
        max_size_ = DEF_EPOLL_SIZE;
        return *this;
    }

    void EPoll::set_max_size(size_t new_size) {
        if (new_size > 0) {
            max_size_ = new_size;
        } else {
            throw EpollError("invalid_new_size");
        }
    }

    void EPoll::add(const Descriptor& fd, uint32_t events) {
        ::epoll_event event{};
        event.events = events;
        event.data.fd = fd.get_fd();

        if (::epoll_ctl(epoll_fd_.get_fd(), EPOLL_CTL_ADD, fd.get_fd(), &event) < 0) {
            throw EpollError("epoll_add_fail");
        }
    }

    void EPoll::mod(const Descriptor& fd, uint32_t events) {
        ::epoll_event event{};
        event.events = events;
        event.data.fd = fd.get_fd();

        if (::epoll_ctl(epoll_fd_.get_fd(), EPOLL_CTL_MOD, fd.get_fd(), &event) < 0) {
            throw EpollError("epoll_mod_fail");
        }
    }

    void EPoll::del(const Descriptor& fd) {
        if (::epoll_ctl(epoll_fd_.get_fd(), EPOLL_CTL_DEL, fd.get_fd(), nullptr) < 0) {
            throw EpollError("epoll_mod_fail");
        }
    }

    std::vector<::epoll_event> EPoll::wait() {
        std::vector<::epoll_event> ret_events(max_size_);
        int ret_events_count = ::epoll_wait(epoll_fd_.get_fd(), ret_events.data(), max_size_, -1);
        if (ret_events_count < 0) {
            throw EpollError("epoll_wait_fail");
        }
        ret_events.resize(ret_events_count);
        return ret_events;
    }

} // namespace net
