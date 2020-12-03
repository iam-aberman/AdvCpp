#ifndef EXAMPLE_LISTENER_H
#define EXAMPLE_LISTENER_H

#include "service_listener.h"


class ExampleListener : public net::IServiceListener {
public:
    void OnNewConnection(net::BufferedConnection& con) override {
        std::cout << "New client on fd #" << con.get_fd().get_fd() << std::endl;
        con.subscribe(EPOLLIN | EPOLLRDHUP);
    }

    void OnWriteDone(net::BufferedConnection& con) override {
        con.unsubscribe(EPOLLOUT);
        con.subscribe(EPOLLIN);
    }
    void OnReadAvailible(net::BufferedConnection& con) override {
        std::cout << "Reading request on #" << con.get_fd().get_fd() << std::endl;

        std::string dest;
        con.read(dest);
        con.write("{ " + dest + " }");

        con.unsubscribe(EPOLLIN);
        con.subscribe(EPOLLOUT);
    }
    void OnError(net::BufferedConnection& con) override {
        std::cerr << "Some error on #" << con.get_fd().get_fd() << std::endl;
    }
    void OnClose(net::BufferedConnection& con) override {
        std::cout << "RDHUP on #" << con.get_fd().get_fd() << std::endl;
        throw net::ServiceFin("");
    }
};

#endif //EXAMPLE_LISTENER_H
