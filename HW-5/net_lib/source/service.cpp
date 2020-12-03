#include "service.h"

namespace net {

    Service::Service(std::weak_ptr<IServiceListener> listener) : listener_(std::move(listener))
    {
    }

    void Service::set_listener(std::weak_ptr<IServiceListener> listener) {
        listener_ = std::move(listener);
    }

    void Service::open(const std::string& address, int port) {
        server_.listen(address, port);
        epoll_.add(server_.get_fd(), EPOLLIN);
    }

    void Service::close() {
        server_.close();
    }

    void Service::close_connection(BufferedConnection& con) {
        int key = con.get_fd().get_fd();
        if (active_connections_.count(key) > 0) {
            active_connections_.erase(key);             // This will close the associated connection
        }
    }

    void Service::run() {
        if (listener_.expired()) {
            throw ServiceError("no_listener_set");
        }

        while (true) {
            std::vector<::epoll_event> ret_events = epoll_.wait();

            for (::epoll_event& event : ret_events) {
                if (event.events & EPOLLERR)  {
                    listener_.lock()->OnError(active_connections_.at(event.data.fd));
                    active_connections_.erase(event.data.fd);

                } else if (event.events & EPOLLRDHUP || event.events & EPOLLHUP) {
                    listener_.lock()->OnClose(active_connections_.at(event.data.fd));
                    active_connections_.erase(event.data.fd);

                } else if (event.events & EPOLLIN) {
                    if (event.data.fd == server_.get_fd().get_fd()) {
                        BufferedConnection new_con(server_.accept(), &epoll_);
                        int key_fd = new_con.get_fd().get_fd();

                        epoll_.add(new_con.get_fd());
                        active_connections_.insert({key_fd, std::move(new_con)});
                        listener_.lock()->OnNewConnection(active_connections_.at(key_fd));

                    } else {
                        BufferedConnection& cur_con = active_connections_.at(event.data.fd);
                        cur_con.read_to_buf();
                        listener_.lock()->OnReadAvailible(active_connections_.at(event.data.fd));
                    }

                } else if (event.events & EPOLLOUT) {
                    BufferedConnection& cur_con = active_connections_.at(event.data.fd);
                    if (!cur_con.get_write_buf().empty()) {
                        cur_con.write_from_buf();
                    }

                    if (cur_con.get_write_buf().empty()) {
                        listener_.lock()->OnWriteDone(cur_con);
                    }
                }
            }
        }
    }

} // namespace net
