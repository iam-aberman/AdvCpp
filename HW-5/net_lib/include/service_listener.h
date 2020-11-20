#ifndef SERVICE_LISTENER_H
#define SERVICE_LISTENER_H

#include "net_exception.h"
#include <iostream>

namespace net {

    class IServiceListener {
    public:
        virtual void OnNewConnection(BufferedConnection& con) = 0;
        virtual void OnWriteDone(BufferedConnection& con) = 0;
        virtual void OnReadAvailible(BufferedConnection& con) = 0;
        virtual void OnError(BufferedConnection& con) = 0;
        virtual void OnClose(BufferedConnection& con) = 0;
    };

    class ExampleListener : public IServiceListener {
    public:
        void OnNewConnection(BufferedConnection& con) override {
            std::cout << "New client on fd #" << con.get_fd().get_fd() << std::endl;
            con.subscribe(EPOLLIN | EPOLLRDHUP);
        }

        void OnWriteDone(BufferedConnection& con) override {
            con.unsubscribe(EPOLLOUT);
            con.subscribe(EPOLLIN);
        }
        void OnReadAvailible(BufferedConnection& con) override {
            std::cout << "Reading request on #" << con.get_fd().get_fd() << std::endl;

            std::string dest;
            con.read(dest);
            con.write("{ " + dest + " }");

            con.unsubscribe(EPOLLIN);
            con.subscribe(EPOLLOUT);
        }
        void OnError(BufferedConnection& con) override {
            std::cerr << "Some error on #" << con.get_fd().get_fd() << std::endl;
        }
        void OnClose(BufferedConnection& con) override {
            std::cout << "RDHUP on #" << con.get_fd().get_fd() << std::endl;
            throw ServiceFin("");
        }
    };

} // namespace net

#endif //SERVICE_LISTENER_H
