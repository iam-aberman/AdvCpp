#include "net_exception.h"
#include "net_epoll.h"

namespace net {

    EPoll::EPoll() : max_size_(DEF_EPOLL_SIZE){
        epoll_fd_ = Descriptor {::epoll_create(1)};
        if (epoll_fd_.get_fd() < 0) {
            throw EpollError("epoll_init_fail");
        }
    }

    void EPoll::add(Descriptor& fd, uint32_t events) {
        ::epoll_event event();
        event.events = events;
        event.data.fd = fd.get_fd();

        if (::epoll_ctl(epoll_fd_.get_fd(), EPOLL_CTL_ADD, fd.get_fd(), &event) < 0) {
            throw EpollError("epoll_add_fail");
        }
    }

    void EPoll::mod(Descriptor& fd, uint32_t events) {
        ::epoll_event event();
        event.events = events;
        event.data.fd = fd.get_fd();

        if (::epoll_ctl(epoll_fd_.get_fd(), EPOLL_CTL_MOD, fd.get_fd(), &event) < 0) {
            throw EpollError("epoll_mod_fail");
        }
    }

    void EPoll::del(Descriptor& fd) {
        if (::epoll_ctl(epoll_fd_.get_fd(), EPOLL_CTL_DEL, fd.get_fd(), nullptr) < 0) {
            throw EpollError("epoll_mod_fail");
        }
    }

}
