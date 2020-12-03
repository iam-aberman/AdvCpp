#ifndef NET_EXCEPTION_H
#define NET_EXCEPTION_H

#include <stdexcept>

namespace net {

    class Exception : public std::runtime_error {
    public:
        using std::runtime_error::runtime_error;
    };

    class EpollError : public Exception {
    public:
        using Exception::Exception;
    };
    
    class BufConnectionError : public Exception {
        using Exception::Exception;
    };

    class ServiceFin : public Exception {
    public:
        using Exception::Exception;
    };

    class ServiceError : public Exception {
    public:
        using Exception::Exception;
    };

} // namespace net

#endif //NET_EXCEPTION_H
