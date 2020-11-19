#include "service.h"

namespace net {

    Service::Service(std::unique_ptr<IServiceListener>&& listener) : listener_(std::move(listener))
    {
    }

    void Service::set_listener(std::unique_ptr<IServiceListener>&& listener) {
        listener_ = std::move(listener);
    }

    void Service::open(const std::string& address, int port) {
        server_.listen(address, port);
        epoll_.add(server_.get_fd(), EPOLLIN);
    }

    void Service::close() {
        server_.close();
    }

    void Service::subscribe_to(BufferedConnection& con, uint32_t events) {
        con.subscribe(events);
    }

    void Service::unsubscribe_from(BufferedConnection& con, uint32_t events) {
        con.unsubscribe(events);
    }

    void Service::run() {
        while (true) {
            std::vector<::epoll_event> ret_events = epoll_.wait();

            for (auto& event : ret_events) {
                if (event.events & EPOLLERR)  {
                    listener_->OnError(active_connections_.at(event.data.fd));
                    active_connections_.erase(event.data.fd);
                    continue;
                }

                if (event.events & EPOLLHUP) {
                    active_connections_.erase(event.data.fd);
                    continue;
                }

                if (event.events & EPOLLRDHUP) {
                    listener_->OnClose(active_connections_.at(event.data.fd));
                    active_connections_.erase(event.data.fd);
                    continue;
                }

                if (event.events & EPOLLIN) {
                    if (event.data.fd == server_.get_fd().get_fd()) {
                        BufferedConnection new_con(server_.accept(), &epoll_);
                        int key_fd = new_con.get_fd().get_fd();

                        epoll_.add(new_con.get_fd(), 0);
                        active_connections_.insert({key_fd, std::move(new_con)});
                        listener_->OnNewConnection(active_connections_.at(key_fd));
                        continue;
                    } else {
                        BufferedConnection& cur_con = active_connections_.at(event.data.fd);
                        cur_con.read_();
                        listener_->OnReadAvailible(active_connections_.at(event.data.fd));
                    }
                }

                if (event.events & EPOLLOUT) {
                    BufferedConnection& cur_con = active_connections_.at(event.data.fd);
                    if (!cur_con.get_write_buf().empty()) {
                        cur_con.write_();
                    }

                    if (cur_con.get_write_buf().empty()) {
                        listener_->OnWriteDone(cur_con);
                    }
                }
            }
        }
    }

}
