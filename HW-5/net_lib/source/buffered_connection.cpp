#include "buffered_connection.h"

namespace net {
    constexpr size_t DEF_BUF_SIZE = 256;

    BufferedConnection::BufferedConnection(tcp::Connection&& tmp, EPoll* epoll) :
    connection_(std::move(tmp)), epoll_(epoll), epoll_flags_(0u)
    {
    }

    void BufferedConnection::read(std::string& dest) {
        dest = read_buf_;
        read_buf_.clear();
    }

    void BufferedConnection::write(const std::string& msg) {
        write_buf_.append(msg);
    }

    std::string& BufferedConnection::get_read_buf() {
        return read_buf_;
    }

    std::string& BufferedConnection::get_write_buf() {
        return write_buf_;
    }

    Descriptor& BufferedConnection::get_fd() {
        return connection_.get_fd();
    }

    void BufferedConnection::subscribe(uint32_t events) {
        epoll_flags_ |= events;
        epoll_->mod(connection_.get_fd(), epoll_flags_);
    }

    void BufferedConnection::unsubscribe(uint32_t events) {
        epoll_flags_ &= (~events);
        epoll_->mod(connection_.get_fd(), epoll_flags_);
    }

    void BufferedConnection::close() {
        connection_.close();
    }

    size_t BufferedConnection::read_from_buf() {
        read_buf_.resize(read_buf_.size() + DEF_BUF_SIZE);
        size_t acquired =
                connection_.read(
                read_buf_.data() + (read_buf_.size() - DEF_BUF_SIZE),
                DEF_BUF_SIZE);
        read_buf_.resize(read_buf_.size() - DEF_BUF_SIZE + acquired);
        return acquired;
    }

    size_t BufferedConnection::write_to_buf() {
        if (!write_buf_.empty()) {
            size_t written =
                    connection_.write(
                    write_buf_.data(), write_buf_.size());
            write_buf_ = write_buf_.substr(written);
            return written;
        }
    }

} // namespace net
